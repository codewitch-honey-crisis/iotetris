using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IoTetris
{
	public partial class Main : Form
	{

		private struct PortData
		{
			public SerialPort Port;
			public PortData(SerialPort port)
			{
				Port = port;
			}
			public override string ToString()
			{
				if (Port != null)
				{
					return Port.PortName;
				}
				return "<null>";
			}
			public override bool Equals(object obj)
			{
				if (!(obj is PortData)) return false;
				PortData other = (PortData)obj;
				return object.Equals(Port, other.Port);
			}
			public override int GetHashCode()
			{
				if (Port == null) return 0;

				return Port.GetHashCode();
			}
		}
		byte[] _data = new byte[1];
		public Main()
		{
			InitializeComponent();
			RefreshSerialBox();
		}
	
		void RefreshSerialBox()
		{
			SerialConnectedCheckBox.Checked = false;
			SerialComboBox.Items.Clear();
			var txt = SerialComboBox.Text;
			foreach(var portName in SerialPort.GetPortNames())
			{
				SerialComboBox.Items.Add(new PortData(new SerialPort(portName, 115200)));
			}
			if(!string.IsNullOrWhiteSpace(txt))
			{
				var i = SerialComboBox.FindString(txt);
				if(i>-1)
				{
					SerialComboBox.SelectedIndex = i;
				} else if (SerialComboBox.Items.Count > 0)
				{
					SerialComboBox.SelectedIndex = 0;
				}
			} else
			{
				if(SerialComboBox.Items.Count > 0)
				{
					SerialComboBox.SelectedIndex = 0;
				}
			}
		}
		void SendSerialKey(Keys keyData)
		{
			if (SerialConnectedCheckBox.Checked && SerialComboBox.SelectedIndex > -1)
			{
				try
				{
					PortData pd = (PortData)SerialComboBox.SelectedItem;
					if(pd.Port.IsOpen)
					{
						try
						{
							switch(keyData)
							{
								case Keys.A:
									_data[0] = 1;
									break;
								case Keys.D:
									_data[0] = 2;
									break;
								case Keys.S:
									_data[0] = 4;
									break;
								case Keys.W:
									_data[0] = 5;
									break;
							}
							pd.Port.Write(_data, 0, 1);
							pd.Port.BaseStream.Flush();
						}
						catch
						{

						}
					}
				}
				catch
				{

				}
			}
		}
		protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
		{
			// if we don't filter out repeats of the <control> Key 
			// there will be a flood of them while the key is held down
			if (keyData == Keys.Control) return false;
			if (keyData == Keys.A)
			{
				SendSerialKey(keyData);
				return true;
			}
			if (keyData == Keys.D)
			{
				SendSerialKey(keyData);
				return true;
			}
			if (keyData == Keys.W)
			{
				SendSerialKey(keyData);
				return true;
			}
			if (keyData == Keys.S)
			{
				SendSerialKey(keyData);
				return true;
			}
			return false;
		}

		private void SerialRefreshButton_Click(object sender, EventArgs e)
		{
			RefreshSerialBox();
		}

		private void SerialConnectedCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			
			if(SerialConnectedCheckBox.Checked)
			{
				if (SerialComboBox.SelectedIndex > -1)
				{
					try
					{
						PortData pd = (PortData)SerialComboBox.SelectedItem;
						pd.Port.Open();
					}
					catch
					{

					}
				}
			} else
			{
				if (SerialComboBox.SelectedIndex > -1)
				{
					try
					{
						PortData pd = (PortData)SerialComboBox.SelectedItem;
						pd.Port.Close();
					}
					catch
					{

					}
				}
			}
		}

		private void PingTimer_Tick(object sender, EventArgs e)
		{
			Invoke(new Action(()=> {
				if (SerialConnectedCheckBox.Checked && SerialComboBox.SelectedIndex > -1)
				{
					try
					{
						PortData pd = (PortData)SerialComboBox.SelectedItem;
						if (pd.Port.IsOpen)
						{
							try
							{
								_data[0] = 6;
								pd.Port.Write(_data, 0, 1);
								pd.Port.BaseStream.Flush();
								System.Diagnostics.Debug.WriteLine("Pinged");
							}
							catch
							{

							}
						}
					}
					catch
					{

					}
				}
			}));
		}
	}
}
