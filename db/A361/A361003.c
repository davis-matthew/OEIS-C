/*
 * A361003  a(n) = A000005(n) + floor((n-1)/2).
 * https://oeis.org/A361003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) + floor((n-1)/2)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000005(n) = number of positive divisors of n */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 for safety */
        return 0;

    /* Compute d(n), the divisor count */
    seq_t d = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            d *= (e + 1);
        }
    }
    if (temp > 1)
        d *= 2;           /* remaining prime factor */

    /* Add floor((n-1)/2) */
    seq_t add = (seq_t)((n - 1) / 2);
    return d + add;
}
