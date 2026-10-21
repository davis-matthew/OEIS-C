/*
 * A247799  a(0) = 1; a(n) = a(n-1) + (if n = a(k) for some k then k else n).
 * https://oeis.org/A247799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 370, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + (n if n is not a previous a(k) else k) */
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq = NULL;          /* store a(0) … a(N) */
static size_t seq_cap = 0;         /* allocated capacity */
static idx_t  seq_len = 0;         /* current number of computed terms */

/* ensure seq has room for indices 0 … n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, sizeof(seq_t) * new_cap);
    if (tmp) {
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* compute a(n) on demand */
seq_t a(idx_t n)
{
    /* initialize on first call */
    if (!seq) {
        seq_cap = 8;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    if (n + 1 > seq_cap)
        ensure_capacity(n);

    /* extend the sequence up to n if needed */
    while (seq_len <= n) {
        idx_t i = seq_len;            /* index for which we are computing a(i) */
        seq_t add = (seq_t)i;         /* default: add i itself */

        /* look for i among previous values a(0)…a(i-1) */
        for (idx_t k = 0; k < i; ++k) {
            if (seq[k] == (seq_t)i) {
                add = (seq_t)k;
                break;
            }
        }

        /* a(i) = a(i-1) + add */
        seq[i] = seq[i - 1] + add;
        seq_len = i + 1;
    }

    return seq[n];
}
