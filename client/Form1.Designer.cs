namespace client
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            bConnect = new Button();
            bSendMessage = new Button();
            bDisconnect = new Button();
            tbMessage = new TextBox();
            rtbResponse = new RichTextBox();
            cbEraseMessage = new CheckBox();
            rtbOutput = new RichTextBox();
            pViews = new Panel();
            menuStrip1 = new MenuStrip();
            statusStrip1 = new StatusStrip();
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            cbResponse = new CheckBox();
            cbOutput = new CheckBox();
            pViews.SuspendLayout();
            SuspendLayout();
            // 
            // bConnect
            // 
            bConnect.Location = new Point(794, 11);
            bConnect.Name = "bConnect";
            bConnect.Size = new Size(75, 23);
            bConnect.TabIndex = 0;
            bConnect.Text = "Connect";
            bConnect.UseVisualStyleBackColor = true;
            bConnect.Click += BConnect_Click;
            // 
            // bSendMessage
            // 
            bSendMessage.Location = new Point(713, 11);
            bSendMessage.Name = "bSendMessage";
            bSendMessage.Size = new Size(75, 51);
            bSendMessage.TabIndex = 1;
            bSendMessage.Text = "Send Message";
            bSendMessage.UseVisualStyleBackColor = true;
            bSendMessage.Click += BSendMessage_Click;
            // 
            // bDisconnect
            // 
            bDisconnect.Location = new Point(794, 39);
            bDisconnect.Name = "bDisconnect";
            bDisconnect.Size = new Size(75, 23);
            bDisconnect.TabIndex = 2;
            bDisconnect.Text = "Disconnect";
            bDisconnect.UseVisualStyleBackColor = true;
            bDisconnect.Click += BDisconnect_Click;
            // 
            // tbMessage
            // 
            tbMessage.Location = new Point(12, 26);
            tbMessage.Name = "tbMessage";
            tbMessage.Size = new Size(695, 23);
            tbMessage.TabIndex = 3;
            // 
            // rtbResponse
            // 
            rtbResponse.Location = new Point(12, 79);
            rtbResponse.Name = "rtbResponse";
            rtbResponse.ReadOnly = true;
            rtbResponse.Size = new Size(695, 105);
            rtbResponse.TabIndex = 4;
            rtbResponse.Text = "";
            // 
            // cbEraseMessage
            // 
            cbEraseMessage.AutoSize = true;
            cbEraseMessage.Location = new Point(561, 8);
            cbEraseMessage.Name = "cbEraseMessage";
            cbEraseMessage.Size = new Size(146, 19);
            cbEraseMessage.TabIndex = 5;
            cbEraseMessage.Text = "clear text after sending";
            cbEraseMessage.UseVisualStyleBackColor = true;
            // 
            // rtbOutput
            // 
            rtbOutput.Location = new Point(12, 217);
            rtbOutput.Name = "rtbOutput";
            rtbOutput.ReadOnly = true;
            rtbOutput.Size = new Size(695, 463);
            rtbOutput.TabIndex = 6;
            rtbOutput.Text = "";
            // 
            // pViews
            // 
            pViews.Controls.Add(cbOutput);
            pViews.Controls.Add(cbResponse);
            pViews.Controls.Add(label3);
            pViews.Controls.Add(label2);
            pViews.Controls.Add(label1);
            pViews.Controls.Add(bSendMessage);
            pViews.Controls.Add(tbMessage);
            pViews.Controls.Add(bConnect);
            pViews.Controls.Add(rtbResponse);
            pViews.Controls.Add(bDisconnect);
            pViews.Controls.Add(rtbOutput);
            pViews.Controls.Add(cbEraseMessage);
            pViews.Dock = DockStyle.Fill;
            pViews.Location = new Point(0, 24);
            pViews.Name = "pViews";
            pViews.Size = new Size(881, 716);
            pViews.TabIndex = 8;
            // 
            // menuStrip1
            // 
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(881, 24);
            menuStrip1.TabIndex = 9;
            menuStrip1.Text = "menuStrip1";
            // 
            // statusStrip1
            // 
            statusStrip1.Location = new Point(0, 718);
            statusStrip1.Name = "statusStrip1";
            statusStrip1.Size = new Size(881, 22);
            statusStrip1.TabIndex = 10;
            statusStrip1.Text = "statusStrip1";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 8);
            label1.Name = "label1";
            label1.Size = new Size(53, 15);
            label1.TabIndex = 7;
            label1.Text = "Message";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(12, 61);
            label2.Name = "label2";
            label2.Size = new Size(57, 15);
            label2.TabIndex = 8;
            label2.Text = "Response";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(12, 199);
            label3.Name = "label3";
            label3.Size = new Size(45, 15);
            label3.TabIndex = 9;
            label3.Text = "Output";
            // 
            // cbResponse
            // 
            cbResponse.AutoSize = true;
            cbResponse.Location = new Point(626, 60);
            cbResponse.Name = "cbResponse";
            cbResponse.Size = new Size(81, 19);
            cbResponse.TabIndex = 10;
            cbResponse.Text = "auto scroll";
            cbResponse.UseVisualStyleBackColor = true;
            // 
            // cbOutput
            // 
            cbOutput.AutoSize = true;
            cbOutput.Location = new Point(626, 198);
            cbOutput.Name = "cbOutput";
            cbOutput.Size = new Size(81, 19);
            cbOutput.TabIndex = 11;
            cbOutput.Text = "auto scroll";
            cbOutput.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(881, 740);
            Controls.Add(statusStrip1);
            Controls.Add(pViews);
            Controls.Add(menuStrip1);
            MainMenuStrip = menuStrip1;
            Name = "Form1";
            Text = "Ledstrip Client";
            Load += Form1_Load;
            pViews.ResumeLayout(false);
            pViews.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button bConnect;
        private Button bSendMessage;
        private Button bDisconnect;
        private TextBox tbMessage;
        private RichTextBox rtbResponse;
        private CheckBox cbEraseMessage;
        private RichTextBox rtbOutput;
        private Panel pViews;
        private MenuStrip menuStrip1;
        private StatusStrip statusStrip1;
        private Label label3;
        private Label label2;
        private Label label1;
        private CheckBox cbOutput;
        private CheckBox cbResponse;
    }
}
