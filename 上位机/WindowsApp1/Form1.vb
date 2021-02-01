Imports System.IO.Ports

Public Class Form1
    Public Structure Cls
        Dim week As Byte
        Dim type As Byte
        Dim start_time As UInt16
        Dim end_time As UInt16
        Dim ring As Byte
        Dim ring_time As Byte
    End Structure
    Private classArr(256) As Cls
    Private classTypes() As String = {"数学", "语文", "英语", "政治", "历史", "地理", "生物", "物理", "化学", "自习", "体育", "班会", "音美"}
    Private classTotal = 0
    Private currentSerial = 1
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If SerialPort1.IsOpen = True Then
            SerialPort1.Close()
        End If
        For i = 1 To 16
            If currentSerial = 16 Then
                currentSerial = 1
            End If
            currentSerial = currentSerial + 1
            SerialPort1.PortName = "COM" + Str(currentSerial)
            SerialPort1.PortName = System.Text.RegularExpressions.Regex.Replace(SerialPort1.PortName, " ", String.Empty, System.Text.RegularExpressions.RegexOptions.IgnoreCase)
            Try
                SerialPort1.Open()
                Button2.Enabled = True
                Button3.Enabled = True
                Button4.Enabled = True
                Button8.Enabled = True
                ToolStripStatusLabel1.Text = "已连接：" + SerialPort1.PortName
                Button1.Text = "尝试下一个"
                Exit For
            Catch
            End Try
        Next i
        If SerialPort1.IsOpen = False Then
            MsgBox("连接失败")
        End If
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        Dim i As Integer
        If SerialPort1.IsOpen = False Then
            Button2.Enabled = False
            Button3.Enabled = False
            Button4.Enabled = False
            Exit Sub
        End If
        classTotal = 0
        ListBox1.Items.Clear()
        Button2.Enabled = False
        Button3.Enabled = False
        Button4.Enabled = False
        ListBox1.Enabled = False
        ProgressBar1.Value = 0
        Try
            SerialPort1.Write("r")
            classTotal = 0
            For i = 0 To 254
                ProgressBar1.Value = Int((i + 1) / 255 * 100)
                classArr(i).week = SerialPort1.ReadByte() - 1
                classArr(i).type = SerialPort1.ReadByte()
                classArr(i).start_time = SerialPort1.ReadByte() * 256
                classArr(i).start_time += SerialPort1.ReadByte()
                classArr(i).end_time = SerialPort1.ReadByte() * 256
                classArr(i).end_time += SerialPort1.ReadByte()
                classArr(i).ring = SerialPort1.ReadByte()
                classArr(i).ring_time = SerialPort1.ReadByte()
                If classArr(i).type < classTypes.Length And Not classArr(i).week = 255 Then
                    classTotal = classTotal + 1
                    ListBox1.Items.Add(classTypes(classArr(i).type))
                End If
                Update()
            Next
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical)
        End Try
        Button2.Enabled = True
        ListBox1.Enabled = True
        Button3.Enabled = True
        Button4.Enabled = True
    End Sub

    Private Sub ListBox1_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ListBox1.SelectedIndexChanged
        If ListBox1.SelectedIndex = -1 Then
            Exit Sub
        End If
        ComboBox1.SelectedIndex = classArr(ListBox1.SelectedIndex).week
        ComboBox2.SelectedIndex = classArr(ListBox1.SelectedIndex).type
        TextBox2.Text = Int(classArr(ListBox1.SelectedIndex).start_time / 60)
        TextBox3.Text = classArr(ListBox1.SelectedIndex).start_time Mod 60
        TextBox4.Text = Int(classArr(ListBox1.SelectedIndex).end_time - classArr(ListBox1.SelectedIndex).start_time)
        ComboBox3.SelectedIndex = classArr(ListBox1.SelectedIndex).ring
        TextBox6.Text = classArr(ListBox1.SelectedIndex).ring_time
    End Sub

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        For i = 0 To classTypes.Length - 1
            ComboBox2.Items.Add(classTypes(i))
        Next
        ComboBox2.SelectedIndex = 0
        ComboBox1.SelectedIndex = 0
        ComboBox3.SelectedIndex = 2
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        Dim dat As Byte
        If SerialPort1.IsOpen = False Then
            Button2.Enabled = False
            Button3.Enabled = False
            Button4.Enabled = False
            Exit Sub
        End If

        Button2.Enabled = False
        Button3.Enabled = False
        Button4.Enabled = False
        ListBox1.Enabled = False
        ProgressBar1.Value = 0
        Try
            SerialPort1.Write("w")
            dat = SerialPort1.ReadByte()
            SerialPort1.BaseStream.WriteByte(classTotal)

            For i = 0 To classTotal - 1
                ProgressBar1.Value = Int((i + 1) / (classTotal) * 100)
                SerialPort1.BaseStream.WriteByte(classArr(i).week + 1)
                SerialPort1.BaseStream.WriteByte(classArr(i).type)
                SerialPort1.BaseStream.WriteByte(Int(classArr(i).start_time \ 256))
                SerialPort1.BaseStream.WriteByte(classArr(i).start_time Mod 256)
                SerialPort1.BaseStream.WriteByte(Int(classArr(i).end_time \ 256))
                SerialPort1.BaseStream.WriteByte(classArr(i).end_time Mod 256)
                SerialPort1.BaseStream.WriteByte(classArr(i).ring)
                SerialPort1.BaseStream.WriteByte(classArr(i).ring_time)
                SerialPort1.ReadByte()
            Next
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical)
        End Try
        Button3.Enabled = True
        Button4.Enabled = True
        Button2.Enabled = True
        ListBox1.Enabled = True
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        ListBox1.Items.Add(ComboBox2.Text)
        classArr(classTotal).type = ComboBox2.SelectedIndex
        classArr(classTotal).week = ComboBox1.SelectedIndex
        classArr(classTotal).start_time = Int(TextBox2.Text) * 60 + Int(TextBox3.Text)
        classArr(classTotal).end_time = Int(TextBox4.Text + classArr(classTotal).start_time)
        classArr(classTotal).ring = ComboBox3.SelectedIndex
        classArr(classTotal).ring_time = Int(TextBox6.Text)
        classTotal = classTotal + 1
        If CheckBox1.Checked = True Then
            Dim i As Integer
            i = classArr(classTotal - 1).start_time
            i = i + Int(TextBox4.Text) + Int(TextBox5.Text)
            TextBox2.Text = Int(i / 60)
            TextBox3.Text = i Mod 60
        End If
    End Sub

    Private Sub Button6_Click(sender As Object, e As EventArgs) Handles Button6.Click
        Dim clstmp As Cls
        Dim sel As Integer
        If ListBox1.SelectedIndex = -1 Or ListBox1.SelectedIndex = 0 Then
            Exit Sub
        End If
        clstmp = classArr(ListBox1.SelectedIndex)
        classArr(ListBox1.SelectedIndex) = classArr(ListBox1.SelectedIndex - 1)
        classArr(ListBox1.SelectedIndex - 1) = clstmp
        sel = ListBox1.SelectedIndex
        ListBox1.Items.RemoveAt(ListBox1.SelectedIndex)
        ListBox1.Items.Insert(sel - 1, classTypes(clstmp.type))
        ListBox1.SelectedIndex = sel - 1
    End Sub

    Private Sub Button7_Click(sender As Object, e As EventArgs) Handles Button7.Click
        Dim clstmp As Cls
        Dim sel As Integer
        If ListBox1.SelectedIndex = -1 Or ListBox1.SelectedIndex = ListBox1.Items.Count - 1 Then
            Exit Sub
        End If
        clstmp = classArr(ListBox1.SelectedIndex)
        classArr(ListBox1.SelectedIndex) = classArr(ListBox1.SelectedIndex + 1)
        classArr(ListBox1.SelectedIndex + 1) = clstmp
        sel = ListBox1.SelectedIndex
        ListBox1.Items.RemoveAt(ListBox1.SelectedIndex)
        ListBox1.Items.Insert(sel + 1, classTypes(clstmp.type))
        ListBox1.SelectedIndex = sel + 1
    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        Dim sel As Integer
        If ListBox1.SelectedIndex = -1 Then
            Exit Sub
        End If
        sel = ListBox1.SelectedIndex
        ListBox1.Items.RemoveAt(sel)
        ListBox1.Items.Insert(sel, classTypes(ComboBox2.SelectedIndex))
        classArr(sel).type = ComboBox2.SelectedIndex
        classArr(sel).week = ComboBox1.SelectedIndex
        classArr(sel).start_time = Int(TextBox2.Text) * 60 + Int(TextBox3.Text)
        classArr(sel).end_time = Int(TextBox4.Text + classArr(sel).start_time)
        classArr(sel).ring = ComboBox3.SelectedIndex
        classArr(sel).ring_time = Int(TextBox6.Text)
        ListBox1.SelectedIndex = sel
    End Sub

    Private Sub Button8_Click(sender As Object, e As EventArgs) Handles Button8.Click
        If SerialPort1.IsOpen = False Then
            Button8.Enabled = False
            Exit Sub
        End If
        Button8.Enabled = False
        SerialPort1.Close()
        Button2.Enabled = False
        Button3.Enabled = False
        Button4.Enabled = False
        Button8.Enabled = False
    End Sub
End Class
