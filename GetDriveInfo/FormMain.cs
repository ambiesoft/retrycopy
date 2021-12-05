using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GetDriveInfo
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();

            txtDriveInfo.Text = Program.Output;
            txtDriveInfo.SelectionStart = 0;
            txtDriveInfo.SelectionLength = 0;

            Text = string.Format("{0} - {1}",
                Program.Drive, Application.ProductName);
        }
    }
}
