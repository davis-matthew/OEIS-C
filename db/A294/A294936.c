/*
 * A294936  Characteristic function for nondeficient numbers (A023196): a(n) =
 *          1 if A001065(n) >= n, 0 otherwise.
 * https://oeis.org/A294936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 1 if sum of proper divisors of n ≥ n, otherwise 0
 * Range:     u8 n <= 9222, u16 n <= 9222, u32 n <= 9222, u64 n <= 9222,
 *            u128 n <= 9222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the characteristic function for non‑deficient numbers */
seq_t a(idx_t n)
{
    /* n == 0 is outside the sequence; n == 1 is not non‑deficient */
    if (n <= 1)
        return 0;

    /* Use 128‑bit arithmetic for the sum to avoid overflow when
       seq_t is small.  The comparison is done in 128‑bit too. */
    unsigned __int128 sum = 1;          /* 1 is always a proper divisor of n>1 */
    uint64_t m = (uint64_t)n;          /* n fits in 64 bits by definition of idx_t */

    /* Enumerate divisors up to sqrt(m).  Each divisor d ≥ 2 contributes
       itself and, if distinct, the complementary divisor m/d. */
    for (uint64_t d = 2; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            sum += d;
            uint64_t other = m / d;
            if (other != d)
                sum += other;
        }
    }

    /* Compare the sum of proper divisors with n.  Return 1 if the
       number is non‑deficient, 0 otherwise. */
    return (sum >= (unsigned __int128)n) ? (seq_t)1 : (seq_t)0;
}
