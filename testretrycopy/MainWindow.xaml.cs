using System;
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
                    @"v143\retrycopy\",
                    "x86",
                    "retrycopy.exe"));
            }
        }
        public MainWindow()
        {
            InitializeComponent();
        }

        int _logIndex = 0;
        void AppendLog(string message)
        {
            txtLog.AppendText( (++_logIndex).ToString() + ":" + message);
            txtLog.AppendText(Environment.NewLine);
            txtLog.SelectionStart = txtLog.Text.Length;
            txtLog.SelectionLength = 0;
            txtLog.CaretIndex = txtLog.Text.Length;
            txtLog.ScrollToEnd();
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
            try
            {
                byte[] ba1 = File.ReadAllBytes(path1);
                byte[] ba2 = File.ReadAllBytes(path2);
                return ba1.SequenceEqual(ba2);
            }
            catch (Exception ex)
            {
                AppendLog(ex.Message);
                return false;
            }
        }
        bool IsSameDirContent(string dir1, string dir2)
        {
            try
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
            catch(Exception ex)
            {
                AppendLog(ex.Message);
                return false;
            }
        }

        enum PathType
        {
            Dir,File,None,
        }

        class PathInfo
        {
            string _path;
            PathType _type;
            byte[] _b;
            public PathInfo(string path, PathType type, byte[] b, bool bNoCreate)
            {
                if (path.StartsWith("\\"))
                    throw new Exception("should not start with \\");
                _path = path;
                _type = type;
                _b = b;
                prepareDirStructure();
            }
            public PathInfo(string path, PathType type, byte[] b):
                this(path, type, b, false)
            { }
            public PathInfo(string path, PathType type):
                this(path, type, null, false)
            { }
            public string ThePath { get { return _path; } }
            public string ParentDir { get { return Path.GetDirectoryName(ThePath); } }
            public bool IsFile { get { return _type == PathType.File; } }
            public bool IsNone { get { return _type == PathType.None; } }
            void prepareDirStructure()
            {
                if (IsNone)
                {
                    SafeDeleteFile(ThePath);
                    return;
                }
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

            public string FileName {  get {  return Path.GetFileName(ThePath); } }
            public string AddFile(string file)
            {
                return Path.Combine(_path, file);
            }
        }
        private void btnCopyFileToNonexistentPath_Click(object sender, RoutedEventArgs e)
        {
            CppUtils.DeleteFile(".\\targetdir");

            var path1 = new PathInfo(".\\testdir\\testfile", PathType.File, GetRandomByte(111));
            var path2 = new PathInfo(".\\targetdir\\targetfile\\testfile", PathType.File);

            StartRetryCopy(string.Format("\"{0}\" -d \"{1}\" -ov no -op copy",
                path1.ThePath, path2.ParentDir));

            AppendLog(IsSameFileContent(
                path1.ThePath, 
                path2.ThePath) ? "OK" : "NG");
        }

        void deepCommon(bool yen)
        {
            var path1 = new PathInfo(".\\testdir\\testfile", PathType.File, GetRandomByte(111));

            CppUtils.DeleteFile(".\\targetdir1");
            string path2 = ".\\targetdir1\\deep1\\deep2" + (yen ? "\\" : "");

            StartRetryCopy(string.Format("{0} -d {1} -ov Yes -op copy",
                path1.ThePath, path2));

            string result = Path.Combine(path2, Path.GetFileName(path1.ThePath));

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
            try
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
            catch(Exception ex)
            {
                AppendLog(ex.Message);
                return false;
            }
        }
        bool IsFileMoved(PathInfo[] paths, string destRoot, int fileCount)
        {
            if (fileCount != Directory.GetFiles(destRoot, "*", SearchOption.AllDirectories).Length)
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
        bool IsFileMoved(PathInfo[] paths, string destRoot)
        {
            return IsFileMoved(paths, destRoot, paths.Length);
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

        private void btnSrcDstSame_Click(object sender, RoutedEventArgs e)
        {
            bool bCopy = true;
            StartRetryCopy(string.Format("\"{0}\" \"{1}\" -d {2}\\ -ov Ask -op " +
             (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move")),
             ".", ".", "."));
        }

        private void btnSameLevelDir_Click(object sender, RoutedEventArgs e)
        {
            bool bCopy = true;

            string dir1 = ".\\dir1";
            CppUtils.DeleteFile(dir1);

            var path11 = new PathInfo(".\\dir1\\file1", PathType.File, GetRandomByte(1000));
            var path12 = new PathInfo(".\\dir1\\file2", PathType.File, GetRandomByte(1000));

            string dir2 = ".\\dir2";
            CppUtils.DeleteFile(dir2);

            var path21 = new PathInfo(".\\dir2\\file1", PathType.None);
            var path22 = new PathInfo(".\\dir2\\file2", PathType.None);

            StartRetryCopy(string.Format("\"{0}\" -d {1}\\ -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move")),
                dir1, dir2));

            if (bCopy)
            {
                AppendLog(IsSameDirContent(dir1, dir2) ? "OK" : "NG");
            }
            else
            {
                AppendLog(!File.Exists(path21.ThePath) ? "OK" : "NG");
                AppendLog(!File.Exists(path22.ThePath) ? "OK" : "NG");
                AppendLog(IsFileMoved(
                    new PathInfo[] { path11, path12, },
                    dir2) ? "OK" : "NG");
            }
        }

        private void btnHasSamePathInSrc_Click(object sender, RoutedEventArgs e)
        {
            bool bCopy = true;

            string dir1 = ".\\dir1";
            CppUtils.DeleteFile(dir1);

            var path11 = new PathInfo(".\\dir1\\file1", PathType.File, GetRandomByte(1000));
            var path12 = new PathInfo(".\\dir1\\file2", PathType.File, GetRandomByte(1000));

            string dir2 = ".\\dir2";
            CppUtils.DeleteFile(dir2);

            var path21 = new PathInfo(".\\dir2\\file1", PathType.None);
            var path22 = new PathInfo(".\\dir2\\file2", PathType.None);

            StartRetryCopy(string.Format("\"{0}\" \"{1}\" -d {2}\\ -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move")),
                dir1, dir1, dir2));

            StartRetryCopy(string.Format("C:\\T K:\\T \"{0}\" C:\\Windows\\ \"{1}\" -d {2}\\ -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move")),
                dir1, dir1, dir2));


        }

        private void btnMockRead_Click(object sender, RoutedEventArgs e)
        {
            string target = "./mockout";
            CppUtils.DeleteFile(target);
            StartRetryCopy(
                string.Format("--use-mockread -d {0} -b 1", target));

            checkMockFile();
        }
        private void btnMockRead_2byte_Click(object sender, RoutedEventArgs e)
        {
            string target = "./mockout";
            CppUtils.DeleteFile(target);
            StartRetryCopy(
                string.Format("--use-mockread -d {0} -b 2", target));

            checkMockFile();
        }
        private void btnMockRead_1024byte_Click(object sender, RoutedEventArgs e)
        {
            string target = "./mockout";
            CppUtils.DeleteFile(target);
            StartRetryCopy(
                string.Format("--use-mockread -d {0} -b 1024", target));

            checkMockFile();
        }
        void checkMockFile()
        {
            try
            {
                DocheckMockFile();
            }
            catch (Exception ex)
            {
                AppendLog(ex.Message);
            }
        }
        void DocheckMockFile()
        { 
            // 10Mb file = 10 * 1024 * 1024;
            // 0-100k=OK (100*1024)
            // (100*1024) + 1024 => bad
            // bat-end = OK
            // all good part will filled with 0xFF
            long FILESIZE = 10 * 1024 * 1024;
            long BADSTARTPOS = (100 * 1024) ;
            int BADSIZE = 4096;

            byte[] fileBytes = File.ReadAllBytes("./mockout/mofile");
            string ret = "";
            ret += "FileSize is ";
            if (fileBytes.Length == FILESIZE)
                ret += "OK";
            else
                ret += "NG";
            ret += " ";

            ret += "top good pos = ";
            bool good = true;
            for(int i=0; i < BADSTARTPOS ;++i)
            {
                if (fileBytes[i] != 0xff)
                    good = false;
            }
            if (good)
                ret += "OK";
            else
                ret += "NG";
            ret += " bad sector=";
            good = true;
            for (long i = BADSTARTPOS; i < BADSTARTPOS + BADSIZE; ++i)
            {
                if (fileBytes[i] != 0x00)
                    good = false;
            }
            if (good)
                ret += "OK";
            else
                ret += "NG";

            ret += " bottom good sector=";
            good = true;
            for (long i = BADSTARTPOS + BADSIZE; i < FILESIZE; ++i)
            {
                if (fileBytes[i] != 0xff)
                    good = false;
            }
            if (good)
                ret += "OK";
            else
                ret += "NG";
            
            AppendLog(ret);
        }

        private void btnUnexistantDirve_Click(object sender, RoutedEventArgs e)
        {
            var src = new PathInfo("./dir1/mysrc", PathType.File, GetRandomByte(100));
            string target = "X:\\T";
            StartRetryCopy(
                string.Format("{0} -d {1}", src.ThePath, target));
        }

        private void btnCopy2FilesWithSameName_Click(object sender, RoutedEventArgs e)
        {
            bool bCopy = true;

            CppUtils.DeleteFile(".\\dir1");
            CppUtils.DeleteFile(".\\dir2");

            var path1 = new PathInfo(".\\dir1\\samefile", PathType.File, GetRandomByte(10));
            var path2 = new PathInfo(".\\dir2\\samefile432432", PathType.File, GetRandomByte(12));
            var path3 = new PathInfo(".\\dir1\\samefile333", PathType.File, GetRandomByte(14));
            var path4 = new PathInfo(".\\dir2\\samefile", PathType.File, GetRandomByte(155));

            string outdir = ".\\outdir\\vvv";
            CppUtils.DeleteFile(outdir);

            StartRetryCopy(string.Format("\"{0}\" \"{1}\" {2} {3} -d {4}\\ -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move")),
                Path.GetFullPath(path1.ThePath),
                Path.GetFullPath(path2.ThePath),
                Path.GetFullPath(path3.ThePath),
                Path.GetFullPath(path4.ThePath),
                Path.GetFullPath(outdir)));
        }

        private void btnMoveToAlreadyExistingDirectory_Click(object sender, RoutedEventArgs e)
        {
            bool bCopy = false;

            CppUtils.DeleteFile(".\\target");
            CppUtils.DeleteFile(".\\source");

            var pathAe1 = new PathInfo(".\\target\\file1", PathType.File, GetRandomByte(10));
            var pathAe2 = new PathInfo(".\\target\\file2", PathType.File, GetRandomByte(10));

            var pathToMove1 = new PathInfo(".\\source\\moving1", PathType.File, GetRandomByte(12));
            var pathToMove2 = new PathInfo(".\\source\\moving2", PathType.File, GetRandomByte(132));
            var pathToMove3 = new PathInfo(".\\source\\moving3", PathType.File, GetRandomByte(122));

            StartRetryCopy(String.Format("{0} {1} {2} -d .\\target -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move")),
                pathToMove1.ThePath, pathToMove2.ThePath, pathToMove3.ThePath));


            AppendLog(IsFileMoved(
                new PathInfo[] { pathToMove1, pathToMove2, pathToMove3 },
                ".\\target",
                5) ? "OK" : "NG");

            AppendLog(File.Exists(pathAe1.ThePath) ? "OK" : "NG");
            AppendLog(File.Exists(pathAe2.ThePath) ? "OK" : "NG");
        }

        private void btnDirectoryOverwrite_Click(object sender, RoutedEventArgs e)
        {
            bool bCopy = false;

            CppUtils.DeleteFile(".\\target");
            CppUtils.DeleteFile(".\\source");

            var pathAe1 = new PathInfo(".\\target\\file1", PathType.File, GetRandomByte(10));
            var pathAe2 = new PathInfo(".\\target\\file2", PathType.File, GetRandomByte(10));
            byte[] bOrigFile3 = GetRandomByte(100);
            var pathAe3 = new PathInfo(".\\target\\file3", PathType.File, bOrigFile3);

            var pathToMove1 = new PathInfo(".\\source\\target\\moving1", PathType.File, GetRandomByte(12));
            byte[] bFile1 = GetRandomByte(100);
            var pathToMoveFile1 = new PathInfo(".\\source\\target\\file1", PathType.File, bFile1);
            byte[] bFile2 = GetRandomByte(1000);
            var pathToMoveFile2 = new PathInfo(".\\source\\target\\file2", PathType.File, bFile2);

            StartRetryCopy(String.Format(".\\source\\target -d . -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move"))));

            AppendLog(bOrigFile3.SequenceEqual(File.ReadAllBytes(".\\target\\file3")) ?
                "OK" : "NG");
            AppendLog(bFile1.SequenceEqual(File.ReadAllBytes(".\\target\\file1")) ?
                "OK" : "NG");
            AppendLog(bFile2.SequenceEqual(File.ReadAllBytes(".\\target\\file2")) ?
                "OK" : "NG");
        }

        private void btnBiggerFileThanFreeSpace_Click(object sender, RoutedEventArgs e)
        {
            bool bCopy = false;

            CppUtils.DeleteFile("X:\\target");
            CppUtils.DeleteFile(".\\source");

            byte[] bFile1 = GetRandomByte(100);
            var pathToMoveFile1 = new PathInfo(".\\source\\file1", PathType.File, bFile1);
            byte[] bFile2 = GetRandomByte(100 * 1024 * 1024);
            var pathToMoveFile2 = new PathInfo(".\\source\\file2", PathType.File, bFile2);

            StartRetryCopy(String.Format(".\\source -d X:\\target -ov Ask -op " +
                (bCopy ? "copy" : (chkRecycle.IsChecked.GetValueOrDefault() ? "moverecycle" : "move"))));
        }
    }
}
