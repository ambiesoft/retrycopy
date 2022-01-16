using Microsoft.WindowsAPICodePack.Dialogs;
using Microsoft.WindowsAPICodePack.Taskbar;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ambiesoft
{
    public class RetrycopyMisc
    {
        public static void SetTaskbarStateNone()
        {
            TaskbarManager.Instance.SetProgressState(TaskbarProgressBarState.NoProgress);
        }
        public static void SetTaskbarStateRunning()
        {
            TaskbarManager.Instance.SetProgressState(TaskbarProgressBarState.Normal);
        }
        public static void SetTaskbarStatePaused()
        {
            TaskbarManager.Instance.SetProgressState(TaskbarProgressBarState.Paused);
        }
        public static void SetTaskProgress(int v)
        {
            TaskbarManager.Instance.SetProgressValue(v, 100);
        }

        static string[] GetMultipleCommon(string title, bool isFolder, string initialDir)
        {
            using (CommonOpenFileDialog dialog = new CommonOpenFileDialog())
            {
                dialog.IsFolderPicker = isFolder;
                dialog.Multiselect = true;
                dialog.Title = title;
                dialog.InitialDirectory = initialDir;
                if (dialog.ShowDialog() != CommonFileDialogResult.Ok)
                    return null;

                return dialog.FileNames.ToArray();
            }
        }
        public static string[] GetMultipleFilesFromUser(string title, string initialDir)
        {
            return GetMultipleCommon(title, false, initialDir);
        }
        public static string[] GetMultipleFoldersFromUser(string title, string initialDir)
        {
            return GetMultipleCommon(title, true, initialDir);
        }
    }
}
