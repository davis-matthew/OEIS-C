/*
 * A306222  Positions of 4's in A306211.
 * https://oeis.org/A306222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 4 in A306211 (A306211 built by
 *            starting with [1] and repeatedly appending the run lengths of
 *            the current sequence; search by generation and scan)
 * Range:     u8 n <= 23, u16 n <= 1481, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 69 terms through n = 8327 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* the word, one byte per letter (it reaches 382601340 letters at n = 25000:
   as int it took 1.5 GB, and growing it failed); its letters are run lengths, small */
static unsigned char *p306222_s = NULL;
static size_t p306222_len = 0, p306222_cap = 0;
static size_t *p306222_pos = NULL;
static size_t p306222_npos = 0, p306222_pcap = 0, p306222_scanned = 0;

static void p306222_step(void)
{
    size_t L = p306222_len;
    if (p306222_cap < 2 * L) {
        size_t nc = 2 * L + 16;
        unsigned char *t = (unsigned char *)realloc(p306222_s, nc);
        if (!t) abort();
        p306222_s = t;
        p306222_cap = nc;
    }
    size_t out = L;
    unsigned run = 1;
    for (size_t i = 0; i + 1 < L; ++i) {
        if (p306222_s[i] == p306222_s[i + 1]) {
            ++run;
        } else {
            if (run > 255) abort();
            p306222_s[out++] = (unsigned char)run;
            run = 1;
        }
    }
    if (run > 255) abort();
    p306222_s[out++] = (unsigned char)run;
    p306222_len = out;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (p306222_s == NULL) {
        p306222_cap = 16;
        p306222_s = (unsigned char *)malloc(p306222_cap);
        if (!p306222_s) abort();
        p306222_s[0] = 1;
        p306222_len = 1;
    }
    while (p306222_npos < (size_t)n) {
        while (p306222_scanned < p306222_len) {
            if (p306222_s[p306222_scanned] == 4) {
                if (p306222_npos >= p306222_pcap) {
                    p306222_pcap = p306222_pcap ? p306222_pcap * 2 : 1024;
                    size_t *t = (size_t *)realloc(p306222_pos, p306222_pcap * sizeof(size_t));
                    if (!t) abort();
                    p306222_pos = t;
                }
                p306222_pos[p306222_npos++] = p306222_scanned;
            }
            ++p306222_scanned;
        }
        if (p306222_npos < (size_t)n) p306222_step();
    }
    return (seq_t)(p306222_pos[(size_t)n - 1] + 1);
}
