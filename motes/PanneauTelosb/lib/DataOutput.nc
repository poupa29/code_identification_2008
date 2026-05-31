
includes special_msg;

interface DataOutput {

  /**
   * @return SUCCESS if the value will be output, FAIL otherwise.
   */
  					   //Taille du message      tableau de d'entier
  command result_t output(uint16_t length ,uint16_t value[BUFFERMAX]);

  /**
   * @return SUCCESS always.
   *
   */
  event result_t outputComplete(result_t success);
}
