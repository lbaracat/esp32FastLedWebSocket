namespace client
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void BConnect_Click(object sender, EventArgs e)
        {
            rtbOutput.Text += "Connecting...\n";

            // Conecta aqui

            // After connect, enable Disconnect button
            bSendMessage.Enabled = true;
            cbEraseMessage.Enabled = true;
            bDisconnect.Enabled = true;
            bConnect.Enabled = false;
        }

        private void BSendMessage_Click(object sender, EventArgs e)
        {
            if (tbMessage.Text != "")
            {
                rtbOutput.Text += "Sending message ";
                rtbOutput.Text += tbMessage.Text;
                rtbOutput.Text += " ...\n";

                // Manda a msg aqui

                // After sending message, clear if needed
                if (cbEraseMessage.Checked)
                {
                    tbMessage.Text = string.Empty;
                }
            }
        }

        private void BDisconnect_Click(object sender, EventArgs e)
        {
            rtbOutput.Text += "Disconnecting...\n";

            // Disconecta aqui

            // After disconnect, enable Connect button
            bSendMessage.Enabled = false;
            cbEraseMessage.Enabled = false;
            bConnect.Enabled = true;
            bDisconnect.Enabled = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            bSendMessage.Enabled = false;
            bDisconnect.Enabled = false;
            cbEraseMessage.Enabled = false;
        }
    }
}
