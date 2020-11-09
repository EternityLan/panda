void default_rx_hook(CAN_FIFOMailBox_TypeDef *to_push) {
  UNUSED(to_push);
}

// *** no output safety mode ***

static void nooutput_init(int16_t param) {
  UNUSED(param);
  controls_allowed = 0;
}

static int nooutput_tx_hook(CAN_FIFOMailBox_TypeDef *to_send) {
  UNUSED(to_send);
  return false;
}

static int nooutput_tx_lin_hook(int lin_num, uint8_t *data, int len) {
  UNUSED(lin_num);
  UNUSED(data);
  UNUSED(len);
  return false;
}

static int default_fwd_hook(int bus_num, CAN_FIFOMailBox_TypeDef *to_fwd) {
  // UNUSED(bus_num);
  UNUSED(to_fwd);
  int bus_fwd = -1;
  // CAN1 to CAN3
  if (bus_num == 0) {
    bus_fwd = 2;
  }
  /*
  if(bus_num == 0){
    // create a timer and measure elapsed time
    uint32_t ts = TIM2->CNT;
    uint32_t ts_elapsed = get_ts_elapsed(ts, eon_detected_last);
    // reset blocking flag if time since we saw the Eon exceeds limit
    if (ts_elapsed > 250000) {
      block = 0;
    }
    // do we see the Eon?
    if(addr == 0x343){
      block = 1;
      eon_detected_last = ts;
    }
    bus_fwd = 2;
  }
  if(bus_num == 2){
    // lock rate to stock 0x343. better have a working DSU!
    if (addr == 0x343){
      button_state = (GET_BYTE(to_fwd, 2) >> 4U);
      send_id(button_state);
    }
    // block cruise message only if it's already being sent on bus 0
    if(!onboot){
      startedtime = TIM2->CNT;
      onboot = 1;
    }
    // DSU normally sends nothing for 2 sec, causing the cruise fault so spam the fake msg
    boot_done = (TIM2->CNT > (startedtime + 2000000));
    if (!boot_done){
      send_spoof_acc();
    }
    int blockmsg = (block | !boot_done) && (addr == 0x343);
    if(!blockmsg){
      bus_fwd = 0;
    }
  }
  */
  return bus_fwd;
}

const safety_hooks nooutput_hooks = {
  .init = nooutput_init,
  .rx = default_rx_hook,
  .tx = nooutput_tx_hook,
  .tx_lin = nooutput_tx_lin_hook,
  .fwd = default_fwd_hook,
};

// *** all output safety mode ***

static void alloutput_init(int16_t param) {
  UNUSED(param);
  controls_allowed = 1;
}

static int alloutput_tx_hook(CAN_FIFOMailBox_TypeDef *to_send) {
  UNUSED(to_send);
  return true;
}

static int alloutput_tx_lin_hook(int lin_num, uint8_t *data, int len) {
  UNUSED(lin_num);
  UNUSED(data);
  UNUSED(len);
  return true;
}

const safety_hooks alloutput_hooks = {
  .init = alloutput_init,
  .rx = default_rx_hook,
  .tx = alloutput_tx_hook,
  .tx_lin = alloutput_tx_lin_hook,
  .fwd = default_fwd_hook,
};
