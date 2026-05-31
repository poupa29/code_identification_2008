
includes special_msg;

configuration DataToRfm
{
  provides {
    interface DataOutput;
    interface StdControl;
  }
}
implementation
{
  components DataToRfmM, GenericComm as Comm;

  DataOutput = DataToRfmM;
  StdControl = DataToRfmM;

  DataToRfmM.Send -> Comm.SendMsg[S_MSG];
  DataToRfmM.SubControl -> Comm;
}

