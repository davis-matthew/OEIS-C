/*
 * A343666  Number of partitions of an n-set without blocks of size 6.
 * https://oeis.org/A343666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (for n = 0) and for n > 0 a(n) = Σ_{k=1..n, k≠6} C(n‑1,
 *            k‑1) · a(n‑k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the e.g.f.  exp(exp(x) – 1 – x^6/6!) and the
   corresponding recurrence for exponential generating functions.  */

#include <stddef.h>  /* for size_t, needed by seq_t macros if any */

static seq_t a_precomputed[10001] = {1};  /* a(0) = 1 */
static idx_t a_size = 1;                 /* number of entries currently stored */

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Extend the precomputed table up to index n if necessary. */
    if (n + 1 > a_size) {
        for (idx_t i = a_size; i <= n; ++i) {
            seq_t sum = 0;
            if (i == 0) {
                sum = (seq_t)1;
            } else {
                seq_t binom = (seq_t)1;               /* C(i-1,0) */
                for (idx_t k = 1; k <= i; ++k) {
                    if (k != 6)                     /* blocks of size 6 are forbidden */
                        sum += binom * a_precomputed[i - k];
                    /* update binomial coefficient for next k */
                    if (k < i) {
                        /* binom(n-1, k) = binom(n-1, k-1) * (n-k) / k */
                        binom = binom * (seq_t)(i - k) / (seq_t)(k);
                    }
                }
            }
            a_precomputed[i] = sum;
        }
        a_size = n + 1;
    }
    return a_precomputed[n];
}
