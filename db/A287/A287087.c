/*
 * A287087  Positions of 0 in A287086.
 * https://oeis.org/A287087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A287086, the fixed point of the
 *            morphism 0->01, 1->22, 2->0 (computed by generating the fixed
 *            point and scanning it for zeros)
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9861 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *ps_s = NULL;
static size_t ps_len = 0, ps_cap = 0, ps_rd = 0, ps_scan = 0;
static uint64_t *ps_pos = NULL;
static size_t ps_np = 0, ps_pcap = 0;

static void ps_push(unsigned char c)
{
    if (ps_len == ps_cap) {
        size_t nc = ps_cap ? ps_cap * 2 : 1024;
        ps_s = (unsigned char *)realloc(ps_s, nc);
        ps_cap = nc;
    }
    ps_s[ps_len++] = c;
}

/* Expand the fixed point by applying the morphism to the next unread letter. */
static void ps_expand(void)
{
    unsigned char c = ps_s[ps_rd++];
    if (c == 0) {
        ps_push(0);
        ps_push(1);
    } else if (c == 1) {
        ps_push(2);
        ps_push(2);
    } else {
        ps_push(0);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (ps_len == 0) {
        /* image of s[0]=0 is "01", which already starts the word */
        ps_push(0);
        ps_push(1);
        ps_rd = 1;
    }
    while (ps_np < (size_t)n) {
        while (ps_scan >= ps_len)
            ps_expand();
        if (ps_s[ps_scan] == 0) {
            if (ps_np == ps_pcap) {
                size_t nc = ps_pcap ? ps_pcap * 2 : 1024;
                ps_pos = (uint64_t *)realloc(ps_pos, nc * sizeof(uint64_t));
                ps_pcap = nc;
            }
            ps_pos[ps_np++] = (uint64_t)ps_scan + 1;
        }
        ps_scan++;
    }
    return (seq_t)ps_pos[n - 1];
}
