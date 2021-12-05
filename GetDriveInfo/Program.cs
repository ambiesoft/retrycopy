using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Ambiesoft;
namespace GetDriveInfo
{
    static class Program
    {
        static StringBuilder output_ = new StringBuilder();
        static StringBuilder err_ = new StringBuilder();
        static string drive_;
        static public void OnOutputReceived(object sender, DataReceivedEventArgs e)
        {
            if(e.Data != null)
                output_.AppendLine(e.Data);
        }
        static public void OnErrReceived(object sender, DataReceivedEventArgs e)
        {
            if(e.Data != null)
                err_.AppendLine(e.Data);
        }

        public static string Output 
        { 
            get 
            {
                return output_.ToString().Replace("\r\n", "\n").Replace("\r", "\n").
                    Replace("\n", "\r\n");
            }
        }
        static public string Drive
        {
            get { return drive_; }
        }
            
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static int Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            if(args.Length!=1)
            {
                CppUtils.Alert("illegal args");
                return 2;
            }
            drive_ = Path.GetPathRoot(args[0]);

            int retval;
            Process process;
            AmbLib.OpenCommandGetResultCallback(
                "fsutil",
                string.Format("fsinfo ntfsinfo {0}", drive_),
                System.Text.Encoding.UTF8,
                out retval,
                new DataReceivedEventHandler(OnOutputReceived),
                new DataReceivedEventHandler(OnErrReceived),
                null,
                out process);
            process.WaitForExit();
            if (retval != 0)
            {
                var message = new StringBuilder();
                message.Append("failed to execute fsutil");
                message.AppendLine();
                if (output_.Length != 0)
                {
                    message.Append(output_);
                    message.AppendLine();
                }
                if (err_.Length != 0)
                {
                    message.Append(err_);
                    message.AppendLine();
                }
                CppUtils.Alert(message.ToString());
                return 1;
            }


            Application.Run(new FormMain());
            return 0;
        }
    }
}
