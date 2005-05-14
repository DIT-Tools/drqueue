using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;

namespace Service_master
{
	public class Service_master : System.ServiceProcess.ServiceBase
	{
		private System.Diagnostics.EventLog eventLog1;
		private System.Timers.Timer timer1;
		/// <summary> 
		/// Variable n�cessaire au concepteur.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Service_master()
		{
			// Cet appel est requis par le Concepteur de composants Windows.Forms.
			InitializeComponent();

			// TODO�: ajoutez les initialisations apr�s l'appel � InitComponent
		}

		// Le point d'entr�e principal pour le processus
		static void Main()
		{
			System.ServiceProcess.ServiceBase[] ServicesToRun;
	
			// Plusieurs services utilisateurs peuvent s'ex�cuter dans le m�me processus. Pour ajouter
			// un autre service � ce processus, modifiez la ligne suivante
			// afin de cr�er un second objet service. Par exemple,
			//
			//   ServicesToRun = New System.ServiceProcess.ServiceBase[] {new Service1(), new MySecondUserService()};
			//
			ServicesToRun = new System.ServiceProcess.ServiceBase[] { new Service_master() };

			System.ServiceProcess.ServiceBase.Run(ServicesToRun);
		}

		/// <summary> 
		/// M�thode requise pour la prise en charge du concepteur - ne modifiez pas 
		/// le contenu de cette m�thode avec l'�diteur de code.
		/// </summary>
		private void InitializeComponent()
		{
			this.eventLog1 = new System.Diagnostics.EventLog();
			this.timer1 = new System.Timers.Timer();
			((System.ComponentModel.ISupportInitialize)(this.eventLog1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.timer1)).BeginInit();
			// 
			// eventLog1
			// 
			this.eventLog1.Log = "Application";
			this.eventLog1.Source = "master";
			// 
			// timer1
			// 
			this.timer1.Enabled = true;
			this.timer1.Interval = 1000;
			this.timer1.Elapsed += new System.Timers.ElapsedEventHandler(this.timer1_Elapsed);
			// 
			// Service_master
			// 
			this.ServiceName = "Service_master";
			((System.ComponentModel.ISupportInitialize)(this.eventLog1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.timer1)).EndInit();

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

		public int elapsed;

		/// <summary>
		/// D�marrage du service.
		/// </summary>
		protected override void OnStart(string[] args)
		{
			//eventLog1.WriteEntry("started !");

			elapsed = 0;
			Process theProcess = new Process();
			theProcess.StartInfo.UseShellExecute = true;
			theProcess.StartInfo.WorkingDirectory = Environment.GetEnvironmentVariable("DRQUEUE_BIN");
			theProcess.StartInfo.CreateNoWindow = true;
			theProcess.StartInfo.FileName = "master.exe"; //theProcess.StartInfo.WorkingDirectory + "\\master.exe";
		    theProcess.StartInfo.Arguments = "-f";
			theProcess.Start();
			eventLog1.WriteEntry(theProcess.StartInfo.FileName + " started !");
		    //try
			//{
		    	//Process []findProcess;
		    	//do
		    	//{
		    		//findProcess = Process.GetProcessesByName("master");
		    	//}
		    	//while (findProcess.Length < 1);
				timer1.Start();
		    //}
			//catch (System.ServiceProcess.TimeoutException)
			//{
			//}
		}
 
		/// <summary>
		/// Arr�t du service.
		/// </summary>
		protected override void OnStop()
		{
			try
			{
				Process []findProcess = Process.GetProcessesByName("master");
				foreach (Process proc in findProcess)
				{
					proc.Kill();
				}
				timer1.Stop();
			}
			catch (System.ServiceProcess.TimeoutException)
			{
				timer1.Stop();
			}	
		}

		private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			elapsed += (int) System.Math.Round(timer1.Interval);
			if (elapsed < 2000)
				return;
			Process []findProcess = Process.GetProcessesByName("master");
			if (findProcess.Length < 1)
			{
				ServiceController ctrl = new ServiceController("drqueue_master");
				ctrl.Stop();
			}		
		}
	}
}
