/*
 * A393226  Maximum difference between b^c and c^b, where b,c are natural
 *          numbers such that b*c=n.
 * https://oeis.org/A393226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{b*c=n} |b^c - c^b| (b,c ∈ ℕ, b*c = n)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t max_diff = 0;  /* a(1) = 0 */

    /* Enumerate divisor pairs (d, e) with d ≤ e, d*e = n. */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d != 0)                /* d not a divisor of n */
            continue;

        idx_t e = n / d;                /* complementary divisor */

        /* Compute d^e and e^d exactly. */
        seq_t pow1 = big_pow((seq_t)d, (seq_t)e);
        seq_t pow2 = big_pow((seq_t)e, (seq_t)d);

        /* Absolute difference. */
        seq_t diff = pow1 > pow2 ? pow1 - pow2 : pow2 - pow1;

        if (diff > max_diff)
            max_diff = diff;
    }

    return max_diff;
}
