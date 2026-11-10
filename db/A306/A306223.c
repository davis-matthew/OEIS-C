/*
 * A306223  Positions of 5's in A306211.
 * https://oeis.org/A306223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 5 in A306211, where A306211 is built
 *            by starting with [1] and repeatedly appending the run lengths of
 *            the current sequence. Computed by direct generation
 * Range:     u8 n <= 1, u16 n <= 5795, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 63 terms through n = 9791 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned int *p306223_s = NULL;
static size_t p306223_len = 0, p306223_cap = 0;
static size_t *p306223_pos = NULL;
static size_t p306223_np = 0, p306223_pcap = 0;

static void p306223_scan(size_t from)
{
    for (size_t i = from; i < p306223_len; ++i) {
        if (p306223_s[i] == 5) {
            if (p306223_np == p306223_pcap) {
                p306223_pcap = p306223_pcap ? p306223_pcap * 2 : 64;
                p306223_pos = (size_t *)realloc(p306223_pos, p306223_pcap * sizeof(size_t));
            }
            p306223_pos[p306223_np++] = i + 1;
        }
    }
}

static void p306223_grow(void)
{
    size_t old = p306223_len;
    /* count runs first */
    size_t runs = 1;
    for (size_t i = 0; i + 1 < old; ++i)
        if (p306223_s[i] != p306223_s[i + 1]) ++runs;
    size_t need = old + runs;
    if (need > p306223_cap) {
        size_t nc = p306223_cap ? p306223_cap : 16;
        while (nc < need) nc *= 2;
        p306223_s = (unsigned int *)realloc(p306223_s, nc * sizeof(unsigned int));
        p306223_cap = nc;
    }
    size_t out = old;
    unsigned int rl = 1;
    for (size_t i = 0; i + 1 < old; ++i) {
        if (p306223_s[i] == p306223_s[i + 1]) ++rl;
        else { p306223_s[out++] = rl; rl = 1; }
    }
    p306223_s[out++] = rl;
    p306223_len = out;
    p306223_scan(old);
}

seq_t a(idx_t n)
{
    if (p306223_s == NULL) {
        p306223_cap = 16;
        p306223_s = (unsigned int *)malloc(p306223_cap * sizeof(unsigned int));
        p306223_s[0] = 1;
        p306223_len = 1;
    }
    while (p306223_np < (size_t)n)
        p306223_grow();
    return (seq_t)p306223_pos[(size_t)n - 1];
}
