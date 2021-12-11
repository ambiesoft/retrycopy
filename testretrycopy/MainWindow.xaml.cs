﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
// using System.Windows.Shapes;
using Ambiesoft;

namespace testretrycopy
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string RetryCopyExe
        {
            get
            {
                return Path.GetFullPath(Path.Combine(
                    Environment.CurrentDirectory,
                    @"..\..\..\..\",
#if DEBUG
                    "Debug",
#else
                    "Release",
#endif
                    @"v142\retrycopy\",
                    "x86",
                    "retrycopy.exe"));
            }
        }
        public MainWindow()
        {
            InitializeComponent();
        }

        void AppendLog(string message)
        {
            txtLog.AppendText(message);
            txtLog.AppendText(Environment.NewLine);
            txtLog.SelectionStart = txtLog.Text.Length;
            txtLog.SelectionLength = 0;
            txtLog.CaretIndex = txtLog.Text.Length;
        }
        void StartRetryCopy(string arg)
        {
            arg += " -start";
            if(chkClose.IsChecked ?? false)
                arg += " -close";
            AppendLog("LaunchWith=" + arg);
            Process proc = Process.Start(RetryCopyExe, arg);
            proc.WaitForExit();
            AppendLog("Done");
        }

        private void btnShowHelp_Click(object sender, RoutedEventArgs e)
        {
            AppendLog("Exe=" + RetryCopyExe);
            Process proc = Process.Start(RetryCopyExe,"-h");
            proc.WaitForExit();
            AppendLog("Done");
        }

        private void btnGitRev_Click(object sender, RoutedEventArgs e)
        {
            StartRetryCopy("--show-gitrev");
        }
        private void btnShowReadError_Click(object sender, RoutedEventArgs e)
        {
            StartRetryCopy("--test-showreaderrordlg");
        }

        byte[] GetRandomByte(int size)
        {
            Random rnd = new Random();
            Byte[] b = new Byte[size];
            rnd.NextBytes(b);
            return b;
        }
  
        static void SafeDeleteFile(string path)
        {
            try
            {
                File.Delete(path);
            }
            catch (Exception) { }
        }

        private bool IsSameFileContent(string path1, string path2)
        {
            byte[] ba1 = File.ReadAllBytes(path1);
            byte[] ba2 = File.ReadAllBytes(path2);
            return ba1.SequenceEqual(ba2);
        }
        bool IsSameDirContent(string dir1, string dir2)
        {
            string[] f1 = Directory.GetFiles(dir1, "*.*", SearchOption.AllDirectories);
            string[] f2 = Directory.GetFiles(dir2, "*.*", SearchOption.AllDirectories);

            if (f1.Length != f2.Length)
                return false;

            for (int i = 0; i < f1.Length; ++i)
            {
                if (!IsSameFileContent(f1[i], f2[i]))
                    return false;
            }
            return true;
        }

        enum PathType
        {
            Dir,File,
        }

        class PathInfo
        {
            string _path;
            PathType _type;
            byte[] _b;
            public PathInfo(string path, PathType type, byte[] b)
            {
                if (path.StartsWith("\\"))
                    throw new Exception("should not start with \\");
                _path = path;
                _type = type;
                _b = b;
                prepareDirStructure();
            }
            public PathInfo(string path, PathType type)
            {
                _path = path;
                _type = type;
                prepareDirStructure();
            }
            public string ThePath { get { return _path; } }
            public bool IsFile { get { return _type == PathType.File; } }
            void prepareDirStructure()
            {
                DirectoryInfo di = new DirectoryInfo(IsFile ? Path.GetDirectoryName(ThePath) : ThePath);
                di.Create();
                if (IsFile)
                {
                    if (_b == null)
                        SafeDeleteFile(ThePath);
                    else
                        File.WriteAllBytes(ThePath, _b);
                }
            }
            public byte[] B { get { return _b; } }
        }
        private void btnCopyFileToNonexistentPath_Click(object sender, RoutedEventArgs e)
        {
            var path1 = new PathInfo(".\\testdir\\testfile", PathType.File, GetRandomByte(111));
            var path2 = new PathInfo(".\\targetdir\\targetfile", PathType.File);

            StartRetryCopy(string.Format("\"{0}\" -d \"{1}\" -ov no -op copy",
                path1.ThePath, path2.ThePath));

            AppendLog(IsSameFileContent(path1.ThePath, path2.ThePath) ? "OK" : "NG");
        }

        void deepCommon(bool yen)
        {
            var path1 = new PathInfo(".\\testdir\\testfile", PathType.File, GetRandomByte(111));

            CppUtils.DeleteFile(".\\targetdir1");
            string path2 = ".\\targetdir1\\deep1\\deep2" + (yen ? "\\" : "");

            StartRetryCopy(string.Format("{0} -d {1} -ov Yes -op copy",
                path1.ThePath, path2));

            string result = yen ? Path.Combine(path2, Path.GetFileName(path1.ThePath)) :
                path2;
            AppendLog(IsSameFileContent(
                path1.ThePath,
                result)
                ? "OK" : "NG");
        }
        private void btnCopyFileToNonExistentPathDeep_Click(object sender, RoutedEventArgs e)
        {
            deepCommon(false);
        }

        private void btnCopyFileToNonExistentPathDeepYen_Click(object sender, RoutedEventArgs e)
        {
            deepCommon(true);
        }

        private void btnDstFileExistButEndWithSep_Click(object sender, RoutedEventArgs e)
        {
            var path1 = new PathInfo(".\\testdir\\testfile", PathType.File, GetRandomByte(111));

            string path2 = ".\\targetdir1\\ddd";
            CppUtils.DeleteFile(path2);
            File.WriteAllText(path2, "a");

            StartRetryCopy(string.Format("\"{0}\" -d {1}\\ -ov Yes",
                Path.GetFullPath(path1.ThePath), Path.GetFullPath(path2)));
        }

        bool IsDirMoved(PathInfo[] paths, string destRoot)
        {
            if (paths.Length != Directory.GetFiles(destRoot, "*", SearchOption.AllDirectories).Length)
                return false;
            foreach (var path in paths)
            {
                string origFile = path.ThePath;
                if (File.Exists(origFile))
                    return false;
                string destFile = Path.Combine(destRoot, path.ThePath);
                if (!File.Exists(destFile))
                    return false;
                byte[] ba2 = File.ReadAllBytes(destFile);
                if (!path.B.SequenceEqual(ba2))
                    return false;
            }
            return true;
        }
        bool IsFileMoved(PathInfo[] paths, string destRoot)
        {
            if (paths.Length != Directory.GetFiles(destRoot, "*", SearchOption.AllDirectories).Length)
                return false;
            foreach (var path in paths)
            {
                string origFile = path.ThePath;
                if (File.Exists(origFile))
                    return false;
                string destFile = Path.Combine(destRoot, Path.GetFileName( path.ThePath));
                if (!File.Exists(destFile))
                    return false;
                byte[] ba2 = File.ReadAllBytes(destFile);
                if (!path.B.SequenceEqual(ba2))
                    return false;
            }
            return true;
        }
        void dirCommon(bool copy)
        {
            string dir1 = ".\\dir1";
            CppUtils.DeleteFile(dir1);

            var path1 = new PathInfo(".\\dir1\\file1", PathType.File, GetRandomByte(1000));
            var path2 = new PathInfo(".\\dir1\\file2", PathType.File, GetRandomByte(1000));
            var path3 = new PathInfo(".\\dir1\\file3", PathType.File, GetRandomByte(0));

            var path12 = new PathInfo(".\\dir1\\dir12\\file12", PathType.File, GetRandomByte(222));

            string outdir = ".\\outdir\\vvv";
            CppUtils.DeleteFile(outdir);

            StartRetryCopy(string.Format("\"{0}\" -d {1}\\ -ov Ask -op " + 
                (copy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle":"move" )),
                Path.GetFullPath(dir1), Path.GetFullPath(outdir)));

            if (copy)
            {
                AppendLog(IsSameDirContent(dir1, Path.Combine(outdir, "dir1"))
                    ? "OK" : "NG");
            }
            else
            {
                AppendLog(!Directory.Exists(dir1) ? "OK" : "NG");
                AppendLog(IsDirMoved(
                    new PathInfo[] { path1, path2, path3, path12 },
                    outdir) ? "OK" : "NG");
            }
        }
        private void btnCopyDir_Click(object sender, RoutedEventArgs e)
        {
            dirCommon(true);
        }

        private void btnMoveDir_Click(object sender, RoutedEventArgs e)
        {
            dirCommon(false);
        }

        void multiCommon(bool bCopy)
        {
            string dir1 = ".\\dir1";
            CppUtils.DeleteFile(dir1);

            var path1 = new PathInfo(".\\dir1\\file1", PathType.File, GetRandomByte(1000));
            var path2 = new PathInfo(".\\dir1\\file2", PathType.File, GetRandomByte(1000));

            string outdir = ".\\outdir\\vvv";
            CppUtils.DeleteFile(outdir);

            StartRetryCopy(string.Format("\"{0}\" \"{1}\" -d {2}\\ -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move")),
                Path.GetFullPath(path1.ThePath),
                Path.GetFullPath(path2.ThePath),
                Path.GetFullPath(outdir)));

            if (bCopy)
            {
                AppendLog(IsSameDirContent(dir1, outdir) ? "OK" : "NG");
            }
            else
            {
                AppendLog(!File.Exists(path1.ThePath) ? "OK" : "NG");
                AppendLog(!File.Exists(path2.ThePath) ? "OK" : "NG");
                AppendLog(IsFileMoved(
                    new PathInfo[] { path1, path2, },
                    outdir) ? "OK" : "NG");
            }
        }
        private void btnCopy2Files_Click(object sender, RoutedEventArgs e)
        {
            multiCommon(true);
        }
        private void btnMove2Files_Click(object sender, RoutedEventArgs e)
        {
            multiCommon(false);
        }
    }
}
