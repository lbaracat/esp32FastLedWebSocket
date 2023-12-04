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
            rtbOutput = new RichTextBox();
            cbEraseMessage = new CheckBox();
            SuspendLayout();
            // 
            // bConnect
            // 
            bConnect.Location = new Point(12, 12);
            bConnect.Name = "bConnect";
            bConnect.Size = new Size(75, 23);
            bConnect.TabIndex = 0;
            bConnect.Text = "Connect";
            bConnect.UseVisualStyleBackColor = true;
            bConnect.Click += BConnect_Click;
            // 
            // bSendMessage
            // 
            bSendMessage.Location = new Point(12, 41);
            bSendMessage.Name = "bSendMessage";
            bSendMessage.Size = new Size(75, 51);
            bSendMessage.TabIndex = 1;
            bSendMessage.Text = "Send Message";
            bSendMessage.UseVisualStyleBackColor = true;
            bSendMessage.Click += BSendMessage_Click;
            // 
            // bDisconnect
            // 
            bDisconnect.Location = new Point(12, 123);
            bDisconnect.Name = "bDisconnect";
            bDisconnect.Size = new Size(75, 23);
            bDisconnect.TabIndex = 2;
            bDisconnect.Text = "Disconnect";
            bDisconnect.UseVisualStyleBackColor = true;
            bDisconnect.Click += BDisconnect_Click;
            // 
            // tbMessage
            // 
            tbMessage.Location = new Point(93, 12);
            tbMessage.Name = "tbMessage";
            tbMessage.Size = new Size(695, 23);
            tbMessage.TabIndex = 3;
            // 
            // rtbOutput
            // 
            rtbOutput.Location = new Point(93, 41);
            rtbOutput.Name = "rtbOutput";
            rtbOutput.ReadOnly = true;
            rtbOutput.Size = new Size(695, 397);
            rtbOutput.TabIndex = 4;
            rtbOutput.Text = "";
            // 
            // cbEraseMessage
            // 
            cbEraseMessage.AutoSize = true;
            cbEraseMessage.Location = new Point(12, 98);
            cbEraseMessage.Name = "cbEraseMessage";
            cbEraseMessage.Size = new Size(76, 19);
            cbEraseMessage.TabIndex = 5;
            cbEraseMessage.Text = "erase text";
            cbEraseMessage.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(cbEraseMessage);
            Controls.Add(rtbOutput);
            Controls.Add(tbMessage);
            Controls.Add(bDisconnect);
            Controls.Add(bSendMessage);
            Controls.Add(bConnect);
            MaximumSize = new Size(816, 489);
            MinimumSize = new Size(816, 489);
            Name = "Form1";
            Text = "Ledstrip Client";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button bConnect;
        private Button bSendMessage;
        private Button bDisconnect;
        private TextBox tbMessage;
        private RichTextBox rtbOutput;
        private CheckBox cbEraseMessage;
    }
}
