/*
 * A369098  Number of permutations of [n] having exactly one adjacent 4-cycle.
 * https://oeis.org/A369098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)-1} (-1)^k * (n-3*k-3)! / k!
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* For n < 4 no 4‑cycle can occur. */
    if (n < 4)
        return 0;

    /* Number of terms in the sum is floor(n/4). */
    idx_t terms = n / 4;
    if (terms == 0)
        return 0;

    /* term_k is stored as an absolute value; its sign is handled separately. */
    __int128 term = ( __int128 ) factorial(n - 3);   /* k = 0 term: (n-3)! */
    __int128 sum  = 0;
    int       sign = 1;   /* start with + for k = 0 */

    for (idx_t k = 0; k < terms; ++k)
    {
        /* Add the current term with the appropriate sign. */
        sum += sign * term;

        /* Prepare term for next k, if any. */
        if (k + 1 < terms)
        {
            idx_t kp1 = k + 1;
            /* denominator = k+1 * (n-3*(k+1)) * (n-3*(k+1)-1) * (n-3*(k+1)-2) */
            __int128 denom = (__int128)kp1 *
                             (n - 3 * kp1) *
                             (n - 3 * kp1 - 1) *
                             (n - 3 * kp1 - 2);

            /* Compute the next absolute term. */
            term = term / denom;

            /* Toggle the sign. */
            sign = -sign;
        }
    }

    /* The result is non‑negative; cast to seq_t. */
    return (seq_t)sum;
}
