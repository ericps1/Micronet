#include "gnu_tcp_socket.h"

int main()
{
    printf("GNU SOCKET SETUP MAIN TEST\n");
    set_freq(128);
    set_bps(2);
    set_mod_rate(80000);
    set_freq_deviation(200);
    set_addr('C');
    set_pkt_len(64);
    set_tx_type(1);
    set_DSA_page(1);
    set_flags(1,1,0);

    clear_chars();
    add_char('-');
    add_char('f');
    add_char(' ');
    add_char('2');
    add_char('0');
    add_char(' ');
    add_char('-');
    add_char('m');
    add_char(' ');
    add_char('8');
    add_char('0');
    configure();

    return 0;
}
