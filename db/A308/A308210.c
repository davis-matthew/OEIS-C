/*
 * A308210  Number of integer-sided triangles with perimeter n and sides a, b
 *          and c such that a <= b <= c and the length of side b is coprime to
 *          n.
 * https://oeis.org/A308210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308210: Number of integer‑sided triangles with perimeter n
 * having sides a <= b <= c and gcd(b,n) = 1.
 *
 * Implementation uses the following characterization:
 * For each b with 1 <= b <= floor((n-1)/2) and gcd(b,n)=1, the number
 * of admissible a is the count of integers a satisfying
 *      max(1, floor(n/2)-b + 1) <= a <= min(b, n-2b).
 * This comes from the triangle inequality a+b > c and the ordering
 * a <= b <= c.
 *
 * The function runs in O(n log n) time (gcd in O(log n)), which
 * easily meets the requirements for n up to 10^4.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    seq_t count = 0;
    idx_t max_b = (n - 1) / 2;           /* b ranges up to floor((n-1)/2) */
    seq_t half   = (seq_t)(n / 2);       /* floor(n/2) */

    for (idx_t b = 1; b <= max_b; ++b) {
        /* only consider b coprime with n */
        if (gcd((seq_t)b, (seq_t)n) != (seq_t)1)
            continue;

        /* lower bound for a: a > n/2 - b  =>  a >= floor(n/2) - b + 1 */
        seq_t L;
        if (half >= (seq_t)b)
            L = half - (seq_t)b + (seq_t)1;
        else
            L = (seq_t)1;

        /* upper bound for a: a <= b and a <= n - 2b */
        seq_t U = (seq_t)(n - 2 * b);
        if (U > (seq_t)b)
            U = (seq_t)b;

        if (L <= U)
            count += U - L + (seq_t)1;
    }

    return count;
}
