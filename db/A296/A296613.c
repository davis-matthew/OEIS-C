/*
 * A296613  Smallest k such that either k >= n and k is a power of 2, or k >=
 *          5n/3 and the prime divisors of k are precisely 2 and 5.
 * https://oeis.org/A296613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 3244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A296613:  smallest k such that
 *     (k >= n  AND  k is a power of 2)  OR
 *     (k >= 5*n/3  AND  the prime divisors of k are exactly 2 and 5)
 * Indexing starts at 1 (a(1) = 1).  For n = 0 the function returns 0.
 * The implementation uses only integer arithmetic and does not depend
 * on any external utilities. */

#include <stdint.h>

/* Return the smallest k satisfying the conditions above for a given n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* behaviour below offset is irrelevant */

    seq_t N = (seq_t)n;

    /* ceil(5*n/3) computed in seq_t */
    seq_t threshold = ((seq_t)N * 5 + 2) / 3;

    /* Candidate 1: next power of two >= n */
    seq_t cand1 = 1;
    while (cand1 < N) cand1 <<= 1;           /* works for any seq_t capacity */

    /* Candidate 2: smallest 2^i * 5^j (i>=1, j>=1) >= threshold */
    seq_t best2 = 0;                         /* 0 indicates unset */

    /* Enumerate powers of two up to a reasonable bound.
       threshold*5 is safely larger than the minimal 2^i*5^j for this n. */
    for (seq_t pow2 = 2; pow2 <= threshold * 5; pow2 <<= 1) {
        seq_t pow5 = 5;
        seq_t k = pow2 * pow5;
        /* Increase the power of five until we meet or exceed the threshold. */
        while (k < threshold) {
            pow5 *= 5;
            k = pow2 * pow5;
        }
        if (best2 == 0 || k < best2) best2 = k;
    }

    /* Return the smaller of the two candidates. */
    return (cand1 < best2) ? cand1 : best2;
}
