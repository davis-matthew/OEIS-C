/*
 * A347543  Number of partitions of n into 7 or more parts.
 * https://oeis.org/A347543
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) – p_{\le6}(n) where p(n) is the number of
 *            unrestricted partitions of n, and p_{\le6}(n) is the number of
 *            partitions with largest part ≤6 (equivalently partitions into at
 *            most 6 parts)
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* indexed by the index */
static seq_t *p_cache = NULL;  /* unrestricted partitions */
static seq_t *q_cache = NULL;  /* partitions with parts ≤6 */
static idx_t   max_computed_n = 0;

/* compute p[n] and q[n] for all 0 ≤ n ≤ N */
static void compute_to(idx_t N)
{
    if (N <= max_computed_n) return;

    /* allocate new arrays, initialized to 0 */
    seq_t *new_p = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *new_q = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));

    /* unrestricted partitions using the classic coin‑change DP */
    new_p[0] = 1;
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t s = i; s <= N; ++s) {
            new_p[s] += new_p[s - i];
        }
    }

    /* partitions with largest part ≤6   (equivalently at most 6 parts) */
    new_q[0] = 1;
    for (idx_t i = 1; i <= 6 && i <= N; ++i) {
        for (idx_t s = i; s <= N; ++s) {
            new_q[s] += new_q[s - i];
        }
    }

    /* replace the old caches */
    free(p_cache);
    free(q_cache);
    p_cache = new_p;
    q_cache = new_q;
    max_computed_n = N;
}

seq_t a(idx_t n)
{
    compute_to(n);               /* ensure we have data up to n */
    seq_t val = p_cache[n] - q_cache[n];
    return val;
}
