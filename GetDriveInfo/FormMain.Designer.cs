﻿
namespace GetDriveInfo
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.txtDriveInfo = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // txtDriveInfo
            // 
            this.txtDriveInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtDriveInfo.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtDriveInfo.Location = new System.Drawing.Point(0, 0);
            this.txtDriveInfo.Multiline = true;
            this.txtDriveInfo.Name = "txtDriveInfo";
            this.txtDriveInfo.ReadOnly = true;
            this.txtDriveInfo.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtDriveInfo.Size = new System.Drawing.Size(571, 366);
            this.txtDriveInfo.TabIndex = 0;
            this.txtDriveInfo.WordWrap = false;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(571, 366);
            this.Controls.Add(this.txtDriveInfo);
            this.Name = "FormMain";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtDriveInfo;
    }
}

