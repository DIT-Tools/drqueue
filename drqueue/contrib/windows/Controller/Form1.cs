using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.ServiceProcess;
using System.Diagnostics;
using System.Resources;
using System.Reflection;

namespace ServicesController
{
	/// <summary>
	/// Description r�sum�e de Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.MenuItem menuItemConnect;
		private System.Timers.Timer timer1;
		private System.Windows.Forms.MenuItem menuItemManager;
		private System.Windows.Forms.NotifyIcon Drqueue_on;
		private System.Windows.Forms.NotifyIcon DrQueue;
		private System.Windows.Forms.ContextMenu contextMenuNotify;
		private System.Windows.Forms.MenuItem menuItemDriver;
		private System.Windows.Forms.MenuItem menuItemSeparator;
		private System.Windows.Forms.MenuItem menuItemQuit;
		private System.Windows.Forms.NotifyIcon Drqueue_off;
		private System.Windows.Forms.Label statusBar1;

		private System.ServiceProcess.ServiceController serviceControllerIpc;

		private const string IPC = "cygserver";
		private const string MASTER = "master";
		private const string SLAVE = "slave";

		public Form1()
		{
			//
			// Requis pour la prise en charge du Concepteur Windows Forms
			//
			InitializeComponent();

			this.serviceControllerIpc = new System.ServiceProcess.ServiceController();
			// 
			// serviceControllerIpc
			// 
			this.serviceControllerIpc.ServiceName = "drqueue_ipc";
		}

		/// <summary>
		/// Nettoyage des ressources utilis�es.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Code g�n�r� par le Concepteur Windows Form
		/// <summary>
		/// M�thode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette m�thode avec l'�diteur de code.
		/// </summary>
		private void InitializeComponent() {
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Form1));
			this.statusBar1 = new System.Windows.Forms.Label();
			this.Drqueue_off = new System.Windows.Forms.NotifyIcon(this.components);
			this.menuItemQuit = new System.Windows.Forms.MenuItem();
			this.menuItemSeparator = new System.Windows.Forms.MenuItem();
			this.menuItemDriver = new System.Windows.Forms.MenuItem();
			this.contextMenuNotify = new System.Windows.Forms.ContextMenu();
			this.DrQueue = new System.Windows.Forms.NotifyIcon(this.components);
			this.Drqueue_on = new System.Windows.Forms.NotifyIcon(this.components);
			this.menuItemManager = new System.Windows.Forms.MenuItem();
			this.timer1 = new System.Timers.Timer();
			this.menuItemConnect = new System.Windows.Forms.MenuItem();
			((System.ComponentModel.ISupportInitialize)(this.timer1)).BeginInit();
			this.SuspendLayout();
			// 
			// statusBar1
			// 
			this.statusBar1.Location = new System.Drawing.Point(8, 8);
			this.statusBar1.Name = "statusBar1";
			this.statusBar1.Size = new System.Drawing.Size(272, 16);
			this.statusBar1.TabIndex = 8;
			// 
			// Drqueue_off
			// 
			this.Drqueue_off.Icon = ((System.Drawing.Icon)(resources.GetObject("Drqueue_off.Icon")));
			this.Drqueue_off.Text = "";
			// 
			// menuItemQuit
			// 
			this.menuItemQuit.Index = 4;
			this.menuItemQuit.Text = "Quit";
			this.menuItemQuit.Click += new System.EventHandler(this.MenuItemQuitClick);
			// 
			// menuItemSeparator
			// 
			this.menuItemSeparator.Enabled = false;
			this.menuItemSeparator.Index = 2;
			this.menuItemSeparator.Text = "-------------";
			// 
			// menuItemDriver
			// 
			this.menuItemDriver.Checked = true;
			this.menuItemDriver.Index = 3;
			this.menuItemDriver.Text = "Driver";
			this.menuItemDriver.Click += new System.EventHandler(this.MenuItemDriverClick);
			// 
			// contextMenuNotify
			// 
			this.contextMenuNotify.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
						this.menuItemConnect,
						this.menuItemManager,
						this.menuItemSeparator,
						this.menuItemDriver,
						this.menuItemQuit});
			// 
			// DrQueue
			// 
			this.DrQueue.ContextMenu = this.contextMenuNotify;
			this.DrQueue.Icon = ((System.Drawing.Icon)(resources.GetObject("DrQueue.Icon")));
			this.DrQueue.Text = "DrQueue";
			this.DrQueue.Visible = true;
			this.DrQueue.MouseDown += new System.Windows.Forms.MouseEventHandler(this.NotifyIcon1MouseDown);
			// 
			// Drqueue_on
			// 
			this.Drqueue_on.Icon = ((System.Drawing.Icon)(resources.GetObject("Drqueue_on.Icon")));
			this.Drqueue_on.Text = "";
			// 
			// menuItemManager
			// 
			this.menuItemManager.Index = 1;
			this.menuItemManager.Text = "Manager";
			this.menuItemManager.Click += new System.EventHandler(this.MenuItemManagerClick);
			// 
			// timer1
			// 
			this.timer1.Enabled = true;
			this.timer1.Interval = 500;
			this.timer1.SynchronizingObject = this;
			this.timer1.Elapsed += new System.Timers.ElapsedEventHandler(this.timer1_Elapsed);
			// 
			// menuItemConnect
			// 
			this.menuItemConnect.Index = 0;
			this.menuItemConnect.Text = "Connected";
			this.menuItemConnect.Click += new System.EventHandler(this.MenuItemConnectClick);
			// 
			// Form1
			// 
			this.AutoScale = false;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 14);
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(288, 29);
			this.Controls.Add(this.statusBar1);
			this.ForeColor = System.Drawing.Color.White;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "Form1";
			this.ShowInTaskbar = false;
			this.Text = "Drqueue Service Controller";
			this.WindowState = System.Windows.Forms.FormWindowState.Minimized;
			this.Closing += new System.ComponentModel.CancelEventHandler(this.Form1Closing);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.Closed += new System.EventHandler(this.Form1Closed);
			((System.ComponentModel.ISupportInitialize)(this.timer1)).EndInit();
			this.ResumeLayout(false);
		}
		#endregion


		/// <summary>
		/// Point d'entr�e principal de l'application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		
		private bool IsActive(string name)
		{
			Process []findProcess = Process.GetProcessesByName(name);
			return (findProcess.Length > 0);
		}

		private void Activate(string name)
		{
			Process theProcess = new Process();
			theProcess.StartInfo.UseShellExecute = true;
			theProcess.StartInfo.WorkingDirectory = Environment.GetEnvironmentVariable("DRQUEUE_BIN");
			theProcess.StartInfo.FileName = name + ".exe";
			theProcess.Start();
		}
				
		private void Activate(string name, string argument)
		{
			Process theProcess = new Process();
			theProcess.StartInfo.UseShellExecute = true;
			theProcess.StartInfo.WorkingDirectory = Environment.GetEnvironmentVariable("DRQUEUE_BIN");
			theProcess.StartInfo.FileName = name + ".exe";
			theProcess.StartInfo.Arguments = argument;
			theProcess.Start();
		}

		private void Kill(string name)
		{
				Process []findProcess = Process.GetProcessesByName(name);
				foreach (Process proc in findProcess)
					proc.Kill();
				System.Threading.Thread.Sleep(1000);
		}

		private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			serviceControllerIpc.Refresh();
			
			if (serviceControllerIpc.Status == System.ServiceProcess.ServiceControllerStatus.Stopped)
					this.menuItemDriver.Checked = false;
			else
					this.menuItemDriver.Checked = true;

			if (!IsActive(SLAVE))
			{
				if (this.menuItemConnect.Checked)
				{
					this.DrQueue.Icon = Drqueue_off.Icon;
					this.menuItemConnect.Checked = false;
				}
			}
				
			else
			{
				if (!this.menuItemConnect.Checked)
				{
					this.DrQueue.Icon = Drqueue_on.Icon;
					this.menuItemConnect.Checked = true;
				}
			}
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
			string MasterName = Environment.GetEnvironmentVariable("DRQUEUE_MASTER");
			string ComputerName = Environment.GetEnvironmentVariable("COMPUTERNAME");
			string IsSlave = Environment.GetEnvironmentVariable("DRQUEUE_ISSLAVE");

			if ((IsSlave == "1") && !IsActive(SLAVE))
			{
				try
				{
					if (serviceControllerIpc.Status == System.ServiceProcess.ServiceControllerStatus.Stopped)
					{
						serviceControllerIpc.Start();
						System.Threading.Thread.Sleep(1000);
					}
				}
				catch(System.Exception ex)
				{
					//statusBar1.Text = ex.Message;
				}
				//Activate(SLAVE, "-f");
			}
			timer1.Start();
		}

		void NotifyIcon1MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == System.Windows.Forms.MouseButtons.Left)
				Activate("drqman");
			/*
			if (this.WindowState == FormWindowState.Normal)
			{
				this.ShowInTaskbar = true;
				this.WindowState = FormWindowState.Minimized;
				this.ShowInTaskbar = false;
			}
			else
				this.WindowState = FormWindowState.Normal;
			*/
		}
		
		void Form1Closed(object sender, System.EventArgs e)
		{
			Kill(SLAVE);
		}

		void Form1Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			/*
			e.Cancel = true;
			this.ShowInTaskbar = true;
			this.WindowState = FormWindowState.Minimized;
			this.ShowInTaskbar = false;			
			*/
		}
		void MenuItemConnectClick(object sender, System.EventArgs e)
		{
			try
			{
				if (!IsActive(SLAVE))
				{
					if (serviceControllerIpc.Status == System.ServiceProcess.ServiceControllerStatus.Stopped)
					{
						serviceControllerIpc.Start();
						System.Threading.Thread.Sleep(1000);
					}
					Activate(SLAVE, "-f");
				}
				else
					Kill(SLAVE);
			}
			catch(System.Exception ex)
			{
				//statusBar1.Text = ex.Message.ToString();
			}
		}
		
		void MenuItemDriverClick(object sender, System.EventArgs e)
		{
			try
			{
				if (serviceControllerIpc.Status == System.ServiceProcess.ServiceControllerStatus.Stopped)
					serviceControllerIpc.Start();
				else
				{
					if (IsActive(SLAVE))
						Kill(SLAVE);										
					if (serviceControllerIpc.Status == System.ServiceProcess.ServiceControllerStatus.Running)
						serviceControllerIpc.Stop();
				}
			}
			catch(System.Exception ex)
			{
				//statusBar1.Text = ex.Message.ToString();
			}
		}
		
		void MenuItemQuitClick(object sender, System.EventArgs e)
		{
			Kill(SLAVE);			
			Application.Exit();
		}
		
		void MenuItemManagerClick(object sender, System.EventArgs e)
		{
			Activate("drqman");
		}
		
	}
}
