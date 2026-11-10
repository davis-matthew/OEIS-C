/*
 * A381841  Position of the n-th occurrence of the digit 3 in A105083(n-1) for
 *          n>=1.
 * https://oeis.org/A381841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position (1-based) of the n-th symbol 3 in the fixed
 *            point 1231121231231... of the morphism 1 -> 12, 2 -> 3, 3 -> 1
 *            (A105083); computed by generating the word in place and scanning
 *            for 3's
 * Range:     u8 n <= 55, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned char *w381841 = NULL;
static size_t w_len = 0, w_cap = 0, w_read = 0, w_scan = 0;
static uint64_t *pos381841 = NULL;
static size_t p_len = 0, p_cap = 0;

static void w_push(unsigned char c)
{
    if (w_len == w_cap) {
        size_t nc = w_cap ? w_cap * 2 : 1024;
        w381841 = (unsigned char *)realloc(w381841, nc);
        w_cap = nc;
    }
    w381841[w_len++] = c;
}

static void p_push(uint64_t v)
{
    if (p_len == p_cap) {
        size_t nc = p_cap ? p_cap * 2 : 256;
        pos381841 = (uint64_t *)realloc(pos381841, nc * sizeof(uint64_t));
        p_cap = nc;
    }
    pos381841[p_len++] = v;
}

seq_t a(idx_t n)
{
    if (w_len == 0) {
        /* fixed point starts with f(1) = 12; expansion continues from w[1] */
        w_push(1);
        w_push(2);
        w_read = 1;
    }
    while (p_len < (size_t)n) {
        while (w_scan >= w_len) {
            unsigned char c = w381841[w_read++];
            if (c == 1) { w_push(1); w_push(2); }
            else if (c == 2) { w_push(3); }
            else { w_push(1); }
        }
        if (w381841[w_scan] == 3) p_push((uint64_t)w_scan + 1);
        w_scan++;
    }
    return (seq_t)pos381841[n - 1];
}
