using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ClassLibraryKSM;

namespace KSS_VI
{
    class KSS_VI:KSM
    {
        int base_codage;
        int passage;
        int reception_number;

        [DllImport("MemoirePartagee.dll", EntryPoint = "?WriteMP@@YAHHPAD@Z")]
        protected static extern int WriteMP(int value, string signetWrite);

        protected void InitBaseCodage()
        {
            base_codage = 1;
            passage = 0;
            reception_number = 0;
        }

        protected override void GetKnowledge(ref Knowledge newKnowledge)
        {
            Console.WriteLine("GetKnowledge");
            base.GetKnowledge(ref newKnowledge);
        }

        protected override void ReadCurrentKnowledge(ref Knowledge newKnowledge)
        {
            Console.WriteLine("ReadCurrentKnowledge");
            base.ReadCurrentKnowledge(ref newKnowledge);
        }

        protected override void ProcessKnowledge(Knowledge newKnowledge)
        {
            switch (newKnowledge.tok)
            {
                case "road_sign":
                    {
                        int id_si = 0;
                        //string uom = "";


                        foreach (Knowledge.TagValue kvp in newKnowledge.tagValue)
                        {
                            switch (kvp.tag)
                            {
                                case "ksm_message_data_produced/observed/observable/road_sign/value/number":
                                    {
                                        base_codage = (base_codage + 1) % 10;
                                        id_si = base_codage * 100000 + int.Parse(kvp.value.ToString());

                                    } break;
                                /*case "ksm_message_data_produced/observed/observable/road_sign/value/unit":
                                    {
                                        uom = kvp.value.ToString();
                                    }break;*/
                            }
                        }



                        if (passage == 0)
                        {
                            Console.WriteLine("(" + reception_number++ + ") ID recu = " + id_si%100000);
                        }
                        passage = (passage + 1) % 5;

                        WriteMP(id_si, "KSS-VI>MAPS");
                    } break;
            }
        }


        static void Main(string[] args)
        {
            KSS_VI sensor = new KSS_VI();
            sensor.Initialize();
            sensor.InitBaseCodage();
            WriteMP(0, "KSS-VI>MAPS");
            System.Console.WriteLine("Press a key to finalize the program...");
            System.Console.ReadKey();
        }   
    }
}
