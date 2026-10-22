/*
 * A398726  Sum of the distinct cycle sizes over all permutations of [n].
 * https://oeis.org/A398726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * A293211(n,k) where A293211(n,k) = number
 *            of permutations of [n] that contain at least one cycle of length
 *            k. The function implements the combinatorial identity a(n) = n!
 *            * Sum_{i=1..n} (-1)^{i+1} / i! * Sum_{k=1}^{⌊n/i⌋} k^{1-i} which
 *            can be evaluated efficiently by iterat...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_val = 0;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t t = 1; t <= n; ++t)
        fact *= (seq_t)t;

    /* main summation */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;
        if (m == 0) continue;   /* floor(n/k)==0 -> no terms */

        seq_t B = fact;            /* B = n! / (i! * k^{i-1}) for current i */
        for (idx_t i = 1; i <= m; ++i) {
            if (i & 1)                 /* (+) for i odd  ->  (-1)^{i+1} = +1 */
                a_val += B;
            else
                a_val -= B;

            if (i < m) {               /* update B for next i */
                /* denom = (i+1) * k */
                seq_t denom = ((seq_t)(i + 1)) * (seq_t)k;
                B /= denom;            /* exact division */
            }
        }
    }
    return a_val;
}
