/*
 * A318868  a(n) = 1^2 + 3^4 + 5^6 + 7^8 + 9^10 + 11^12 + 13^14 + ... + (up to
 *          n).
 * https://oeis.org/A318868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*floor((n-1)/2) + 1)*(n mod 2) + Σ_{i=1..floor(n/2)}
 *            (2*i – 1)^(2*i)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, not required for n≥1 */

    seq_t last_even = 0;   /* holds a(2k-2) while running */
    seq_t last_odd  = 0;   /* holds a(2k-1) after an odd step */

    for (idx_t i = 1; i <= n; ++i)
    {
        if (i & 1)  /* i is odd → a(i) = a(i-1) + i */
        {
            seq_t cur = last_even + (seq_t)i;
            last_odd = cur;
        }
        else        /* i is even → a(i) = a(i-2) + (2*k-1)^(2*k) */
        {
            idx_t k   = i >> 1;                 /* k = i/2 */
            seq_t base = (seq_t)(2 * k - 1);    /* odd base */
            seq_t exp  = (seq_t)(2 * k);        /* even exponent */
            seq_t term = big_pow(base, exp);    /* (2k-1)^(2k) */
            seq_t cur = last_even + term;
            last_even = cur;
        }
    }

    return (n & 1) ? last_odd : last_even;
}
