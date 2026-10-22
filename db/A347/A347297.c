/*
 * A347297  a(1) = 1; for n >= 1, if a(n) is even then a(n+1) = a(n) / 2,
 *          otherwise, say a(n) is the k-th odd term in the sequence, a(n+1) =
 *          a(n) + k.
 * https://oeis.org/A347297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n ≥ 1, if a(n) is even then a(n+1) = a(n)/2;
 *            otherwise let k be the ordinal of a(n) among the odd terms of
 *            the sequence (the first odd term is counted as k = 1), then
 *            a(n+1) = a(n) + k. Memoised recurrence – values are computed on
 *            demand and cached for subsequent calls
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are provided by the build system. */

static seq_t *seq_buf = NULL;   /* dynamic array holding computed values   */
static size_t seq_cap = 0;      /* current allocated capacity (in elements) */
static idx_t seq_len = 0;       /* number of values currently stored       */
static size_t odd_cnt = 0;      /* number of odd terms seen so far          */

/* Ensure the buffer can hold at least 'need' elements. */
static inline void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety: undefined for n < 1 */

    /* Initialise on first invocation. */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq_buf[0] = (seq_t)1;         /* a(1) = 1 */
        seq_len = 1;
        odd_cnt = 1;                   /* first term is odd */
    }

    /* Extend the cache until we have computed a(n). */
    while (seq_len < n) {
        seq_t cur  = seq_buf[seq_len - 1];
        seq_t next;

        /* Use modulo 2 for parity tests; works for any integer type. */
        if ((cur % (seq_t)2) == 0) {        /* cur is even */
            next = cur / 2;
        } else {                            /* cur is odd */
            /* cur is the odd_cnt‑th odd term (odd_cnt already counts cur) */
            next = cur + (seq_t)odd_cnt;
        }

        /* If the new term is odd, increment the odd counter. */
        if ((next % (seq_t)2) != 0) {
            odd_cnt++;
        }

        ensure_capacity(seq_len + 1);
        seq_buf[seq_len] = next;
        seq_len++;
    }

    return seq_buf[n - 1];
}
