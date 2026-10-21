/*
 * A289239  Fixed point of the mapping 00->0010, 01->100, 10->010, starting
 *          with 00.
 * https://oeis.org/A289239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Fixed point of the mapping:
        00 -> 0010
        01 -> 100
        10 -> 010
   Starting with the word "00".  The function returns the n‑th
   bit (n ≥ 1) of the resulting infinite word. */
#include <stdint.h>
#include <stdlib.h>

static char *cur = NULL;        /* current word buffer            */
static char *nxt = NULL;        /* temporary buffer for next word */
static size_t cur_len = 0;      /* length of current word         */
static size_t cur_cap = 0;      /* capacity of cur buffer         */
static size_t nxt_cap = 0;      /* capacity of nxt buffer         */

/* Ensure cur has room for at least 'needed' characters.   */
static void ensure_cur_cap(size_t needed)
{
    if (cur_cap < needed) {
        size_t new_cap = cur_cap ? cur_cap * 2 : 2;
        if (new_cap < needed) new_cap = needed;
        cur = (char *)realloc(cur, new_cap);
        cur_cap = new_cap;
    }
}

/* Ensure nxt has room for at least 'needed' characters.   */
static void ensure_nxt_cap(size_t needed)
{
    if (nxt_cap < needed) {
        size_t new_cap = nxt_cap ? nxt_cap * 2 : 2;
        if (new_cap < needed) new_cap = needed;
        nxt = (char *)realloc(nxt, new_cap);
        nxt_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined; return 0 for safety          */
    /* Initialise from the starting word "00" on first call. */
    if (cur_len == 0) {
        ensure_cur_cap(2);
        cur[0] = '0';
        cur[1] = '0';
        cur_len = 2;
    }
    /* Grow the word until it contains at least n bits. */
    while (cur_len < (size_t)n) {
        size_t pairs = cur_len / 2;                    /* number of non‑overlapping pairs */
        size_t required = pairs * 4;                   /* worst case buffer size after expansion */
        ensure_nxt_cap(required);
        size_t w = 0;                                  /* write index in nxt */

        for (size_t i = 0; i + 1 < cur_len; i += 2) {
            if (cur[i] == '0' && cur[i + 1] == '0') {   /* 00 -> 0010 */
                nxt[w++] = '0';
                nxt[w++] = '0';
                nxt[w++] = '1';
                nxt[w++] = '0';
            } else if (cur[i] == '0' && cur[i + 1] == '1') { /* 01 -> 100 */
                nxt[w++] = '1';
                nxt[w++] = '0';
                nxt[w++] = '0';
            } else { /* cur[i] == '1' && cur[i+1] == '0'  -> 10 -> 010 */
                nxt[w++] = '0';
                nxt[w++] = '1';
                nxt[w++] = '0';
            }
        }
        /* Ignore an unmatched trailing symbol if cur_len is odd. */

        /* Swap the buffers and capacities. */
        char *tmp_buf = cur;  cur = nxt;  nxt = tmp_buf;
        size_t tmp_cap = cur_cap; cur_cap = nxt_cap; nxt_cap = tmp_cap;

        cur_len = w;          /* new length of the word */
    }
    return (seq_t)(cur[n - 1] - '0');  /* return the n-th bit as seq_t */
}
