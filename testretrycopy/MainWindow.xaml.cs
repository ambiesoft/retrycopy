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
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            txtLog.AppendText("Test Started" + Environment.NewLine);
            txtLog.AppendText("Test Ended");
        }

        private void btnShowHelp_Click(object sender, RoutedEventArgs e)
        {
            AppendLog("Exe=" + RetryCopyExe);
            Process proc = Process.Start(RetryCopyExe,"-h");
            proc.WaitForExit();
            AppendLog("Done");
        }

        void StartRetryCopy(string arg)
        {
            AppendLog("LaunchWith=" + arg);
            Process proc = Process.Start(RetryCopyExe, arg);
            proc.WaitForExit();
            AppendLog("Done");
        }
        private void btnGitRev_Click(object sender, RoutedEventArgs e)
        {
            StartRetryCopy("--show-gitrev");
        }

        byte[] GetRandomByte(int size)
        {
            Random rnd = new Random();
            Byte[] b = new Byte[size];
            rnd.NextBytes(b);
            return b;
        }
        void PrepareFile()
        {
        }

        private void btnSetSrcDst_DstNotExists_Click(object sender, RoutedEventArgs e)
        {
            string dir = ".\\testdir";
            Directory.CreateDirectory(dir);
            string file = "testfile";
            string path1 = Path.Combine(dir, file);
            File.WriteAllBytes(path1, GetRandomByte(1024));

            string path2 = ".\\targetdir1\\aaa";
            File.Delete(path2);

            StartRetryCopy(string.Format("-s \"{0}\" -d \"{1}\"",
                path1, path2));

            AppendLog(IsSameFileContent(path1, path2) ? "OK" : "NG");
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
        private void btnSetSrcDst_DstIsDir_Click(object sender, RoutedEventArgs e)
        {
            string dir = ".\\testdir";
            CppUtils.DeleteFile(dir);

            Directory.CreateDirectory(dir);
            string file = "testfile";
            string path1 = Path.Combine(dir, file);
            File.WriteAllBytes(path1, GetRandomByte(1024));

            CppUtils.DeleteFile(".\\targetdir1");
            string path2 = ".\\targetdir1\\ddd";
            Directory.CreateDirectory(path2);

            StartRetryCopy(string.Format("-s \"{0}\" -d \"{1}\" -ov Yes",
                path1, path2));

            AppendLog(IsSameFileContent(path1, 
                Path.Combine(path2, "testfile")) ? "OK" : "NG");
        }

        private void btnDstNotExistEndWithSep_Click(object sender, RoutedEventArgs e)
        {
            string dir = ".\\testdir";
            CppUtils.DeleteFile(dir);

            Directory.CreateDirectory(dir);
            string file = "testfile";
            string path1 = Path.Combine(dir, file);
            File.WriteAllBytes(path1, GetRandomByte(1024));

            string path2 = ".\\targetdir1\\ddd\\";
            CppUtils.DeleteFile(path2);

            StartRetryCopy(string.Format("-s \"{0}\" -d {1} -ov Yes",
                path1, path2));

            AppendLog(IsSameFileContent(path1,
                Path.Combine(path2, "testfile")) ? "OK" : "NG");
        }

        private void btnDstFileExistButEndWithSep_Click(object sender, RoutedEventArgs e)
        {
            string dir = ".\\testdir";
            CppUtils.DeleteFile(dir);

            Directory.CreateDirectory(dir);
            string file = "testfile";
            string path1 = Path.Combine(dir, file);
            File.WriteAllBytes(path1, GetRandomByte(1024));

            string path2 = ".\\targetdir1\\ddd";
            CppUtils.DeleteFile(path2);
            File.WriteAllText(path2, "a");

            StartRetryCopy(string.Format("-s \"{0}\" -d {1}\\ -ov Yes",
                Path.GetFullPath( path1), Path.GetFullPath( path2)));
        }

        private void btnCopyDir_Click(object sender, RoutedEventArgs e)
        {
            string dir1 = ".\\dir1";
            CppUtils.DeleteFile(dir1);
            Directory.CreateDirectory(dir1);
            string file1 = Path.Combine(dir1, "file1");
            File.WriteAllBytes(file1, GetRandomByte(1000));
            string file2 = Path.Combine(dir1, "file2");
            File.WriteAllBytes(file2, GetRandomByte(1000));
            string file3 = Path.Combine(dir1, "file3");
            File.WriteAllBytes(file3, GetRandomByte(0));

            string dir12 = Path.Combine(dir1, "dir12");
            Directory.CreateDirectory(dir12);
            string file12 = Path.Combine(dir12, "file12");
            File.WriteAllBytes(file12, GetRandomByte(222));

            string outdir = ".\\outdir\\vvv";
            CppUtils.DeleteFile(outdir);

            StartRetryCopy(string.Format("-s \"{0}\" -d {1}\\ -ov Ask",
                Path.GetFullPath(dir1), Path.GetFullPath(outdir)));

            AppendLog(IsSameDirContent(dir1, Path.Combine(outdir, "dir1"))
                ? "OK" : "NG");

        }

        private void btnMoveDir_Click(object sender, RoutedEventArgs e)
        {
            string dir1 = ".\\dir1";
            CppUtils.DeleteFile(dir1);
            Directory.CreateDirectory(dir1);
            string file1 = Path.Combine(dir1, "file1");
            File.WriteAllBytes(file1, GetRandomByte(1000));
            string file2 = Path.Combine(dir1, "file2");
            File.WriteAllBytes(file2, GetRandomByte(1000));
            string file3 = Path.Combine(dir1, "file3");
            File.WriteAllBytes(file3, GetRandomByte(0));

            string dir12 = Path.Combine(dir1, "dir12");
            Directory.CreateDirectory(dir12);
            string file12 = Path.Combine(dir12, "file12");
            File.WriteAllBytes(file12, GetRandomByte(222));

            string outdir = ".\\outdir\\vvv";
            CppUtils.DeleteFile(outdir);

            StartRetryCopy(string.Format("-s \"{0}\" -d {1}\\ -ov Ask -rm YesDelete",
                Path.GetFullPath(dir1), Path.GetFullPath(outdir)));

            AppendLog(!Directory.Exists(dir1) ? "OK" : "NG");
        }
    }
}
