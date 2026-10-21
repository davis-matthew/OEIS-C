/*
 * A277551  Numbers k such that k/5^m == 2 (mod 5), where 5^m is the greatest
 *          power of 5 that divides k.
 * https://oeis.org/A277551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computes numbers k such that k/5^m ≡ 2 (mod 5) by scanning
 *            integers
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;     /* cache of sequence values */
static idx_t  seq_cnt = 0;    /* number of elements stored */
static idx_t  seq_cap = 0;    /* current capacity of seq */
static seq_t  next_test = 1;  /* next integer to test */

/* Grow the cache to accommodate at least 'need' elements. */
static void ensure_capacity(idx_t need) {
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) { /* allocation failure: abort (environment dependent) */
        abort();
    }
    seq_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset handling */
    /* Generate sequence entries until we have at least n of them. */
    while (seq_cnt < n) {
        seq_t val = next_test;
        seq_t tmp = val;
        /* Remove all trailing 5's from the number */
        while ((tmp % 5) == 0) tmp /= 5;
        if ((tmp % 5) == 2) {
            ensure_capacity(seq_cnt + 1);
            seq[seq_cnt++] = val;
        }
        ++next_test;
    }
    return seq[n - 1];
}
