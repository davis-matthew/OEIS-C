/*
 * A381866  Number of labeled histories for rooted 5-furcating trees with 4n+1
 *          leaves if simultaneous 5-furcations are not allowed.
 * https://oeis.org/A381866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4*n+1)!/120^n) * Π_{i=1..n} (4*i-3) Recurrence: a(0) =
 *            1 a(n+1) = (4*n+5)*(4*n+3)*(4*n+1)*(2*n+1)*(n+1)/15 * a(n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base case */
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t val = (seq_t)1;

    /* Compute iteratively using the recurrence. */
    for (idx_t k = (idx_t)0; k < n; ++k) {
        /* Five consecutive factors in the recurrence */
        seq_t f[5];
        f[0] = ((seq_t)4) * k + ((seq_t)5); /* 4*k + 5 */
        f[1] = ((seq_t)4) * k + ((seq_t)3); /* 4*k + 3 */
        f[2] = ((seq_t)4) * k + ((seq_t)1); /* 4*k + 1 */
        f[3] = ((seq_t)2) * k + ((seq_t)1); /* 2*k + 1 */
        f[4] = ((seq_t)k) + ((seq_t)1);     /* k + 1 */

        /* Divide one factor by 3 (exact division) */
        int i;
        for (i = 0; i < 5; ++i) {
            if (f[i] % ((seq_t)3) == (seq_t)0) {
                f[i] /= ((seq_t)3);
                break;
            }
        }

        /* Divide one factor by 5 (exact division) */
        for (i = 0; i < 5; ++i) {
            if (f[i] % ((seq_t)5) == (seq_t)0) {
                f[i] /= ((seq_t)5);
                break;
            }
        }

        /* Multiply the five factors into the running product */
        for (i = 0; i < 5; ++i)
            val *= f[i];
    }

    return val;
}
