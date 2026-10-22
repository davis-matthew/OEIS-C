/*
 * A320999  Related to the enumeration of pseudo-square convex polyominoes by
 *          semiperimeter.
 * https://oeis.org/A320999
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, 3 ≤ d ≤ n/2} (n/d – 1) * (d – 2)
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) return 0;   /* values below the offset are undefined */

    seq_t result = 0;
    idx_t half = n >> 1;   /* n/2 */

    /* enumerate all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* first divisor d1 */
            if (d1 >= 3 && d1 <= half)
            {
                seq_t q = (seq_t)(n / d1);
                result += (q - 1) * (seq_t)(d1 - 2);
            }

            /* second divisor d2, skip if it equals d1 */
            if (d2 != d1 && d2 >= 3 && d2 <= half)
            {
                seq_t q = (seq_t)(n / d2);
                result += (q - 1) * (seq_t)(d2 - 2);
            }
        }
    }

    return result;
}
