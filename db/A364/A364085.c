/*
 * A364085  Expansion of Sum_{k>0} k * x^k / (1 - x^(4*k-1)).
 * https://oeis.org/A364085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Sum_{d|4*n-1, d ≡ 3 (mod 4)} (d+1)
 * Range:     u8 n <= 48, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1: a(1) is first term; for n=0 return 0 */
        return 0;

    /* m = 4*n - 1  (must fit in idx_t) */
    idx_t m = 4 * n - 1;
    seq_t sum = 0;

    /* Enumerate all divisors d of m */
    /* use i <= m / i to avoid overflow when checking i*i <= m */
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            /* first divisor d1 = i */
            idx_t d1 = i;
            if ((d1 & 3) == 3)            /* d1 ≡ 3 (mod 4) */
                sum += (seq_t)d1 + 1;

            /* second divisor d2 = m/i, distinct from d1 */
            idx_t d2 = m / i;
            if (d2 != d1 && (d2 & 3) == 3)
                sum += (seq_t)d2 + 1;
        }
    }

    /* sum is guaranteed to be divisible by 4; shift right 2 bits */
    return sum >> 2;
}
