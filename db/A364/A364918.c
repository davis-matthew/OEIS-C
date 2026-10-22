/*
 * A364918  a(1) = 1; for n >= 2, a(n) = a(n - GCD(n - 1, a(n - 1))) + GCD(n,
 *          a(n - 1)).
 * https://oeis.org/A364918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 405, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   for n ≥ 2, a(n) = a(n − GCD(n − 1, a(n − 1))) + GCD(n, a(n − 1)) */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Computes the n‑th term of A364918.
   The function is independent for each call and meets the
   requirements for all supported seq_t sizes. */
seq_t a(idx_t n)
{
    /* Out of range: OEIS offset is 1, but a caller could pass 0. */
    if (n == 0) return (seq_t)0;

    /* Base case a(1) */
    if (n == 1) return (seq_t)1;

    /* Allocate a temporary array of size n + 1 (index 0 unused). */
    size_t sz = (size_t)(n + 1);
    seq_t *seq = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!seq) return (seq_t)0;          /* allocation failure → return 0 */

    /* Initialise a(1) */
    seq[1] = (seq_t)1;

    /* Compute terms iteratively from 2 to n. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t prev = seq[i - 1];
        /* offset = GCD(i-1, a(i-1)) */
        idx_t offset = (idx_t)gcd((seq_t)(i - 1), prev);
        /* j = i - offset ≥ 1 because offset ≤ i-1 */
        idx_t j = i - offset;

        /* a(i) = a(j) + GCD(i, a(i-1)) */
        seq[i] = seq[j] + gcd((seq_t)i, prev);
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
