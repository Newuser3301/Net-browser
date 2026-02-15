using System;
using System.Windows;

namespace NetBrowser.Shell;

public partial class MainWindow : Window
{
  private readonly Services.CoreIpcClient _ipc = new();

  public MainWindow()
  {
    InitializeComponent();
    _ipc.OnLine += s => Dispatcher.Invoke(() => LogBox.AppendText(s + Environment.NewLine));
    _ipc.ConnectAsync();
  }

  private void Ping_Click(object sender, RoutedEventArgs e)
  {
    _ipc.Send($"PING|{_ipc.NextId()}");
  }

  private void Go_Click(object sender, RoutedEventArgs e)
  {
    var url = UrlBox.Text.Trim();
    if (url.Length == 0) return;
    _ipc.Send($"NAV|{_ipc.NextId()}|{url}");
  }
}