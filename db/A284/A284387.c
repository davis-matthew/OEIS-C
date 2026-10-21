/*
 * A284387  {010->2}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A284387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the unique fixed point of the morphism * 0 → 10 * 1 → 2 *
 *            2 → 2210 * starting from a single symbol 1. * * The function
 *            builds the word by repeatedly applying the morphism * until the
 *            requested index is contained in the resulting sequence. * It
 *            keeps a static prefix that is grown only when...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Allocation helpers that work in both C and C++ */
#ifdef __cplusplus
#  define ALLOC_TSEQ(n)   new seq_t[(size_t)(n)]
#  define DEALLOC_TSEQ(p) delete[] (p)
#else
#  define ALLOC_TSEQ(n)   ((seq_t*)malloc((size_t)(n) * sizeof(seq_t)))
#  define DEALLOC_TSEQ(p) free(p)
#endif

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;   /* array of symbols of the word */
    static size_t seq_len = 0;    /* current number of symbols */

    if (n == 0)          /* function is only called with n >= 1 */
        return (seq_t)0;

    /* Initialise with the first non‑trivial word: f(1) = 2 */
    if (seq == NULL) {
        seq     = ALLOC_TSEQ(1);
        seq[0]  = (seq_t)2;
        seq_len = 1;
    }

    /* Grow the sequence until it contains at least n symbols */
    while (seq_len < (size_t)n) {
        /* Compute length of the next iteration */
        size_t new_len = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t cur = seq[i];
            if ((int)cur == 0)          new_len += 2;   /* 0 → 1 0   */
            else if ((int)cur == 1)     new_len += 1;   /* 1 → 2     */
            else /* cur == 2 */         new_len += 4;   /* 2 → 2 2 1 0 */
        }

        /* Build next iteration */
        seq_t *next = ALLOC_TSEQ(new_len);
        size_t pos = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t cur = seq[i];
            if ((int)cur == 0) {
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
            } else if ((int)cur == 1) {
                next[pos++] = (seq_t)2;
            } else { /* cur == 2 */
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
            }
        }

        /* Free old sequence and continue with the new one */
        DEALLOC_TSEQ(seq);
        seq   = next;
        seq_len = new_len;
    }

    return seq[n - 1];
}
