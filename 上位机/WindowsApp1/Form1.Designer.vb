<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form 重写 Dispose，以清理组件列表。
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Windows 窗体设计器所必需的
    Private components As System.ComponentModel.IContainer

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。  
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.Button1 = New System.Windows.Forms.Button()
        Me.ListBox1 = New System.Windows.Forms.ListBox()
        Me.ComboBox2 = New System.Windows.Forms.ComboBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Button2 = New System.Windows.Forms.Button()
        Me.Button3 = New System.Windows.Forms.Button()
        Me.ComboBox3 = New System.Windows.Forms.ComboBox()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.Button4 = New System.Windows.Forms.Button()
        Me.TextBox2 = New System.Windows.Forms.TextBox()
        Me.TextBox3 = New System.Windows.Forms.TextBox()
        Me.TextBox4 = New System.Windows.Forms.TextBox()
        Me.TextBox6 = New System.Windows.Forms.TextBox()
        Me.ComboBox1 = New System.Windows.Forms.ComboBox()
        Me.Button5 = New System.Windows.Forms.Button()
        Me.Button6 = New System.Windows.Forms.Button()
        Me.Button7 = New System.Windows.Forms.Button()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.CheckBox1 = New System.Windows.Forms.CheckBox()
        Me.TextBox5 = New System.Windows.Forms.TextBox()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.Label9 = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.ToolStripStatusLabel1 = New System.Windows.Forms.ToolStripStatusLabel()
        Me.ProgressBar1 = New System.Windows.Forms.ToolStripProgressBar()
        Me.Button8 = New System.Windows.Forms.Button()
        Me.StatusStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'SerialPort1
        '
        Me.SerialPort1.ReadBufferSize = 40960
        Me.SerialPort1.ReadTimeout = 2000
        Me.SerialPort1.WriteBufferSize = 20480
        '
        'Button1
        '
        Me.Button1.Location = New System.Drawing.Point(30, 11)
        Me.Button1.Margin = New System.Windows.Forms.Padding(2)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(74, 20)
        Me.Button1.TabIndex = 1
        Me.Button1.Text = "自动连接"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'ListBox1
        '
        Me.ListBox1.FormattingEnabled = True
        Me.ListBox1.ItemHeight = 12
        Me.ListBox1.Location = New System.Drawing.Point(30, 34)
        Me.ListBox1.Margin = New System.Windows.Forms.Padding(2)
        Me.ListBox1.Name = "ListBox1"
        Me.ListBox1.Size = New System.Drawing.Size(175, 244)
        Me.ListBox1.TabIndex = 3
        '
        'ComboBox2
        '
        Me.ComboBox2.FormattingEnabled = True
        Me.ComboBox2.Location = New System.Drawing.Point(356, 65)
        Me.ComboBox2.Margin = New System.Windows.Forms.Padding(2)
        Me.ComboBox2.Name = "ComboBox2"
        Me.ComboBox2.Size = New System.Drawing.Size(152, 20)
        Me.ComboBox2.TabIndex = 5
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(324, 44)
        Me.Label2.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(29, 12)
        Me.Label2.TabIndex = 6
        Me.Label2.Text = "星期"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(324, 67)
        Me.Label3.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(29, 12)
        Me.Label3.TabIndex = 7
        Me.Label3.Text = "课程"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(303, 91)
        Me.Label4.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(53, 12)
        Me.Label4.TabIndex = 9
        Me.Label4.Text = "上课时间"
        '
        'Button2
        '
        Me.Button2.Enabled = False
        Me.Button2.Location = New System.Drawing.Point(30, 290)
        Me.Button2.Margin = New System.Windows.Forms.Padding(2)
        Me.Button2.Name = "Button2"
        Me.Button2.Size = New System.Drawing.Size(74, 35)
        Me.Button2.TabIndex = 10
        Me.Button2.Text = "读取"
        Me.Button2.UseVisualStyleBackColor = True
        '
        'Button3
        '
        Me.Button3.Enabled = False
        Me.Button3.Location = New System.Drawing.Point(130, 290)
        Me.Button3.Margin = New System.Windows.Forms.Padding(2)
        Me.Button3.Name = "Button3"
        Me.Button3.Size = New System.Drawing.Size(74, 35)
        Me.Button3.TabIndex = 11
        Me.Button3.Text = "保存"
        Me.Button3.UseVisualStyleBackColor = True
        '
        'ComboBox3
        '
        Me.ComboBox3.FormattingEnabled = True
        Me.ComboBox3.Items.AddRange(New Object() {"响一声", "倒计时", "静音"})
        Me.ComboBox3.Location = New System.Drawing.Point(356, 114)
        Me.ComboBox3.Margin = New System.Windows.Forms.Padding(2)
        Me.ComboBox3.Name = "ComboBox3"
        Me.ComboBox3.Size = New System.Drawing.Size(152, 20)
        Me.ComboBox3.TabIndex = 12
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(303, 116)
        Me.Label5.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(53, 12)
        Me.Label5.TabIndex = 14
        Me.Label5.Text = "设置提醒"
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Location = New System.Drawing.Point(302, 139)
        Me.Label6.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(53, 12)
        Me.Label6.TabIndex = 15
        Me.Label6.Text = "提前提醒"
        '
        'Button4
        '
        Me.Button4.Enabled = False
        Me.Button4.Location = New System.Drawing.Point(356, 160)
        Me.Button4.Margin = New System.Windows.Forms.Padding(2)
        Me.Button4.Name = "Button4"
        Me.Button4.Size = New System.Drawing.Size(152, 29)
        Me.Button4.TabIndex = 16
        Me.Button4.Text = "存入列表"
        Me.Button4.UseVisualStyleBackColor = True
        '
        'TextBox2
        '
        Me.TextBox2.Location = New System.Drawing.Point(358, 89)
        Me.TextBox2.Margin = New System.Windows.Forms.Padding(2)
        Me.TextBox2.Name = "TextBox2"
        Me.TextBox2.Size = New System.Drawing.Size(21, 21)
        Me.TextBox2.TabIndex = 17
        Me.TextBox2.Text = "7"
        '
        'TextBox3
        '
        Me.TextBox3.Location = New System.Drawing.Point(394, 89)
        Me.TextBox3.Margin = New System.Windows.Forms.Padding(2)
        Me.TextBox3.Name = "TextBox3"
        Me.TextBox3.Size = New System.Drawing.Size(20, 21)
        Me.TextBox3.TabIndex = 17
        Me.TextBox3.Text = "0"
        '
        'TextBox4
        '
        Me.TextBox4.Location = New System.Drawing.Point(475, 88)
        Me.TextBox4.Margin = New System.Windows.Forms.Padding(2)
        Me.TextBox4.Name = "TextBox4"
        Me.TextBox4.Size = New System.Drawing.Size(34, 21)
        Me.TextBox4.TabIndex = 17
        Me.TextBox4.Text = "40"
        '
        'TextBox6
        '
        Me.TextBox6.Location = New System.Drawing.Point(356, 137)
        Me.TextBox6.Margin = New System.Windows.Forms.Padding(2)
        Me.TextBox6.Name = "TextBox6"
        Me.TextBox6.Size = New System.Drawing.Size(152, 21)
        Me.TextBox6.TabIndex = 18
        Me.TextBox6.Text = "0"
        '
        'ComboBox1
        '
        Me.ComboBox1.FormattingEnabled = True
        Me.ComboBox1.Items.AddRange(New Object() {"一", "二", "三", "四", "五", "六", "日"})
        Me.ComboBox1.Location = New System.Drawing.Point(356, 42)
        Me.ComboBox1.Margin = New System.Windows.Forms.Padding(2)
        Me.ComboBox1.Name = "ComboBox1"
        Me.ComboBox1.Size = New System.Drawing.Size(152, 20)
        Me.ComboBox1.TabIndex = 4
        '
        'Button5
        '
        Me.Button5.Location = New System.Drawing.Point(356, 194)
        Me.Button5.Margin = New System.Windows.Forms.Padding(2)
        Me.Button5.Name = "Button5"
        Me.Button5.Size = New System.Drawing.Size(152, 27)
        Me.Button5.TabIndex = 20
        Me.Button5.Text = "修改当前"
        Me.Button5.UseVisualStyleBackColor = True
        '
        'Button6
        '
        Me.Button6.Location = New System.Drawing.Point(208, 58)
        Me.Button6.Margin = New System.Windows.Forms.Padding(2)
        Me.Button6.Name = "Button6"
        Me.Button6.Size = New System.Drawing.Size(23, 79)
        Me.Button6.TabIndex = 21
        Me.Button6.Text = "↑"
        Me.Button6.UseVisualStyleBackColor = True
        '
        'Button7
        '
        Me.Button7.Location = New System.Drawing.Point(208, 160)
        Me.Button7.Margin = New System.Windows.Forms.Padding(2)
        Me.Button7.Name = "Button7"
        Me.Button7.Size = New System.Drawing.Size(23, 75)
        Me.Button7.TabIndex = 22
        Me.Button7.Text = "↓"
        Me.Button7.UseVisualStyleBackColor = True
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Location = New System.Drawing.Point(420, 91)
        Me.Label7.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(53, 12)
        Me.Label7.TabIndex = 23
        Me.Label7.Text = "持续时间"
        '
        'CheckBox1
        '
        Me.CheckBox1.AutoSize = True
        Me.CheckBox1.Checked = True
        Me.CheckBox1.CheckState = System.Windows.Forms.CheckState.Checked
        Me.CheckBox1.Location = New System.Drawing.Point(439, 230)
        Me.CheckBox1.Margin = New System.Windows.Forms.Padding(2)
        Me.CheckBox1.Name = "CheckBox1"
        Me.CheckBox1.Size = New System.Drawing.Size(72, 16)
        Me.CheckBox1.TabIndex = 24
        Me.CheckBox1.Text = "自动递增"
        Me.ToolTip1.SetToolTip(Me.CheckBox1, "单击""存入列表""后上课时间自动增加持续时间和课间时长")
        Me.CheckBox1.UseVisualStyleBackColor = True
        '
        'TextBox5
        '
        Me.TextBox5.Location = New System.Drawing.Point(356, 226)
        Me.TextBox5.Margin = New System.Windows.Forms.Padding(2)
        Me.TextBox5.Name = "TextBox5"
        Me.TextBox5.Size = New System.Drawing.Size(48, 21)
        Me.TextBox5.TabIndex = 25
        Me.TextBox5.Text = "10"
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Location = New System.Drawing.Point(301, 230)
        Me.Label8.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(53, 12)
        Me.Label8.TabIndex = 26
        Me.Label8.Text = "课间时长"
        '
        'ToolTip1
        '
        Me.ToolTip1.ToolTipTitle = "提示"
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Location = New System.Drawing.Point(406, 233)
        Me.Label9.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(29, 12)
        Me.Label9.TabIndex = 27
        Me.Label9.Text = "分钟"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Location = New System.Drawing.Point(380, 93)
        Me.Label10.Margin = New System.Windows.Forms.Padding(2, 0, 2, 0)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(11, 12)
        Me.Label10.TabIndex = 28
        Me.Label10.Text = ":"
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ToolStripStatusLabel1, Me.ProgressBar1})
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 332)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Size = New System.Drawing.Size(534, 22)
        Me.StatusStrip1.TabIndex = 29
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'ToolStripStatusLabel1
        '
        Me.ToolStripStatusLabel1.AutoSize = False
        Me.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1"
        Me.ToolStripStatusLabel1.Size = New System.Drawing.Size(134, 17)
        Me.ToolStripStatusLabel1.Text = "串口未连接"
        '
        'ProgressBar1
        '
        Me.ProgressBar1.Name = "ProgressBar1"
        Me.ProgressBar1.Size = New System.Drawing.Size(100, 16)
        '
        'Button8
        '
        Me.Button8.Enabled = False
        Me.Button8.Location = New System.Drawing.Point(130, 11)
        Me.Button8.Name = "Button8"
        Me.Button8.Size = New System.Drawing.Size(75, 20)
        Me.Button8.TabIndex = 30
        Me.Button8.Text = "断开连接"
        Me.Button8.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(534, 354)
        Me.Controls.Add(Me.Button8)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.Label10)
        Me.Controls.Add(Me.Label9)
        Me.Controls.Add(Me.Label8)
        Me.Controls.Add(Me.TextBox5)
        Me.Controls.Add(Me.CheckBox1)
        Me.Controls.Add(Me.Label7)
        Me.Controls.Add(Me.Button7)
        Me.Controls.Add(Me.Button6)
        Me.Controls.Add(Me.Button5)
        Me.Controls.Add(Me.TextBox6)
        Me.Controls.Add(Me.TextBox4)
        Me.Controls.Add(Me.TextBox3)
        Me.Controls.Add(Me.TextBox2)
        Me.Controls.Add(Me.Button4)
        Me.Controls.Add(Me.Label6)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.ComboBox3)
        Me.Controls.Add(Me.Button3)
        Me.Controls.Add(Me.Button2)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.ComboBox2)
        Me.Controls.Add(Me.ComboBox1)
        Me.Controls.Add(Me.ListBox1)
        Me.Controls.Add(Me.Button1)
        Me.Margin = New System.Windows.Forms.Padding(2)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "课程管理工具"
        Me.StatusStrip1.ResumeLayout(False)
        Me.StatusStrip1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents SerialPort1 As IO.Ports.SerialPort
    Friend WithEvents Button1 As Button
    Friend WithEvents ListBox1 As ListBox
    Friend WithEvents ComboBox2 As ComboBox
    Friend WithEvents Label2 As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Button2 As Button
    Friend WithEvents Button3 As Button
    Friend WithEvents ComboBox3 As ComboBox
    Friend WithEvents Label5 As Label
    Friend WithEvents Label6 As Label
    Friend WithEvents Button4 As Button
    Friend WithEvents TextBox2 As TextBox
    Friend WithEvents TextBox3 As TextBox
    Friend WithEvents TextBox4 As TextBox
    Friend WithEvents TextBox6 As TextBox
    Friend WithEvents ComboBox1 As ComboBox
    Friend WithEvents Button5 As Button
    Friend WithEvents Button6 As Button
    Friend WithEvents Button7 As Button
    Friend WithEvents Label7 As Label
    Friend WithEvents CheckBox1 As CheckBox
    Friend WithEvents TextBox5 As TextBox
    Friend WithEvents Label8 As Label
    Friend WithEvents ToolTip1 As ToolTip
    Friend WithEvents Label9 As Label
    Friend WithEvents Label10 As Label
    Friend WithEvents StatusStrip1 As StatusStrip
    Friend WithEvents ToolStripStatusLabel1 As ToolStripStatusLabel
    Friend WithEvents ProgressBar1 As ToolStripProgressBar
    Friend WithEvents Button8 As Button
End Class
