namespace IoTetris
{
	partial class Main
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
			this.components = new System.ComponentModel.Container();
			this.label1 = new System.Windows.Forms.Label();
			this.SerialComboBox = new System.Windows.Forms.ComboBox();
			this.SerialRefreshButton = new System.Windows.Forms.Button();
			this.SerialConnectedCheckBox = new System.Windows.Forms.CheckBox();
			this.label2 = new System.Windows.Forms.Label();
			this.PingTimer = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(13, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(31, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "Port";
			// 
			// SerialComboBox
			// 
			this.SerialComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.SerialComboBox.FormattingEnabled = true;
			this.SerialComboBox.Location = new System.Drawing.Point(51, 13);
			this.SerialComboBox.Name = "SerialComboBox";
			this.SerialComboBox.Size = new System.Drawing.Size(121, 24);
			this.SerialComboBox.TabIndex = 1;
			// 
			// SerialRefreshButton
			// 
			this.SerialRefreshButton.Location = new System.Drawing.Point(179, 13);
			this.SerialRefreshButton.Name = "SerialRefreshButton";
			this.SerialRefreshButton.Size = new System.Drawing.Size(75, 23);
			this.SerialRefreshButton.TabIndex = 2;
			this.SerialRefreshButton.Text = "Refresh";
			this.SerialRefreshButton.UseVisualStyleBackColor = true;
			this.SerialRefreshButton.Click += new System.EventHandler(this.SerialRefreshButton_Click);
			// 
			// SerialConnectedCheckBox
			// 
			this.SerialConnectedCheckBox.AutoSize = true;
			this.SerialConnectedCheckBox.Location = new System.Drawing.Point(261, 13);
			this.SerialConnectedCheckBox.Name = "SerialConnectedCheckBox";
			this.SerialConnectedCheckBox.Size = new System.Drawing.Size(94, 20);
			this.SerialConnectedCheckBox.TabIndex = 3;
			this.SerialConnectedCheckBox.Text = "Connected";
			this.SerialConnectedCheckBox.UseVisualStyleBackColor = true;
			this.SerialConnectedCheckBox.CheckedChanged += new System.EventHandler(this.SerialConnectedCheckBox_CheckedChanged);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(13, 40);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(187, 16);
			this.label2.TabIndex = 4;
			this.label2.Text = "W/A/S/D (drop/left/rotate/right)";
			// 
			// PingTimer
			// 
			this.PingTimer.Enabled = true;
			this.PingTimer.Interval = 500;
			this.PingTimer.Tick += new System.EventHandler(this.PingTimer_Tick);
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(372, 78);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.SerialConnectedCheckBox);
			this.Controls.Add(this.SerialRefreshButton);
			this.Controls.Add(this.SerialComboBox);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.Name = "Main";
			this.Text = "IoTetris";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox SerialComboBox;
		private System.Windows.Forms.Button SerialRefreshButton;
		private System.Windows.Forms.CheckBox SerialConnectedCheckBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Timer PingTimer;
	}
}

