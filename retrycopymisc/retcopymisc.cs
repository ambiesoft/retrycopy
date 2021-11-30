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
    }
}
