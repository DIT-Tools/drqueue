Imports System.ServiceProcess

Public Class Service1
    Inherits System.ServiceProcess.ServiceBase

#Region " Code g�n�r� par le Concepteur de composants "

    Public Sub New()
        MyBase.New()

        ' Cet appel est requis par le Concepteur de composants.
        InitializeComponent()

        ' Ajoutez une initialisation quelconque apr�s l'appel InitializeComponent()

    End Sub

    'La m�thode substitu�e Dispose du UserService pour nettoyer la liste des composants.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    ' Le point d'entr�e principal pour le processus
    <MTAThread()> _
    Shared Sub Main()
        Dim ServicesToRun() As System.ServiceProcess.ServiceBase

        ' Plusieurs services NT s'ex�cutent dans le m�me processus. Pour ajouter
        ' un autre service � ce processus, modifiez la ligne suivante
        ' pour cr�er un second objet service. Par exemple,
        '
        '   ServicesToRun = New System.ServiceProcess.ServiceBase () {New Service1, New MySecondUserService}
        '
        ServicesToRun = New System.ServiceProcess.ServiceBase () {New Service1}

        System.ServiceProcess.ServiceBase.Run(ServicesToRun)
    End Sub

    'Requis par le Concepteur de composants
    Private components As System.ComponentModel.IContainer

    ' REMARQUE�: la proc�dure suivante est requise par le Concepteur de composants
    ' Elle peut �tre modifi�e � l'aide du Concepteur de composants.  
    ' Ne la modifiez pas en utilisant l'�diteur de code.
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        '
        'Service1
        '
        Me.CanShutdown = True
        Me.ServiceName = "Service1"

    End Sub

#End Region


    Dim theProcess As New Process

    Protected Overrides Sub OnStart(ByVal args() As String)
        ' Ajoutez ici le code pour d�marrer votre service. Cette m�thode doit permettre � votre
        ' service de faire son travail.

        theProcess.StartInfo.UseShellExecute = True
        theProcess.StartInfo.FileName = "ipc-daemon2.exe"
        theProcess.StartInfo.WorkingDirectory = Environment.GetEnvironmentVariable("DRQUEUE_BIN")
        theProcess.Start()
    End Sub

    Protected Overrides Sub OnStop()
        ' Ajoutez ici le code pour effectuer les destructions n�cessaires � l'arr�t de votre service.

        ' Dim findProcess As Process() = Process.GetProcessesByName("ipc-daemon2")

        ' For Each proc As Process In findProcess
        ' proc.Kill()
        ' Next

        'theProcess.StartInfo.UseShellExecute = True
        'theProcess.StartInfo.FileName = "pskill.exe"
        'theProcess.StartInfo.Arguments = "ipc-daemon2.exe"
        'theProcess.StartInfo.WorkingDirectory = Environment.GetEnvironmentVariable("DRQUEUE_BIN")
        'theProcess.Start()

        'Dim shell
        'shell = CreateObject("wscript.shell")
        'shell.run("%DRQUEUE_ROOT%\bin\pskill.exe ipc-daemon2.exe")
        'shell = Nothing

    End Sub

End Class
