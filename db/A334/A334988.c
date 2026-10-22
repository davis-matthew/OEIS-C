/*
 * A334988  Sum of tetrahedral numbers dividing n.
 * https://oeis.org/A334988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of tetrahedral numbers dividing n a(n) = Σ_{t|n, t ∈
 *            {1,4,10,20,35,56,…}} t where t(k) = binomial(k+2,3) =
 *            k(k+1)(k+2)/6
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        /* compute tetrahedral number t(k) = k(k+1)(k+2)/6 safely */
        uint64_t a = k;
        uint64_t b = k + 1;
        uint64_t c = k + 2;

        /* divide by 2 */
        if ((a & 1ULL) == 0)
            a >>= 1;
        else if ((b & 1ULL) == 0)
            b >>= 1;
        else
            c >>= 1;           /* c is even when a and b are odd */

        /* divide by 3 */
        if (a % 3ULL == 0)
            a /= 3;
        else if (b % 3ULL == 0)
            b /= 3;
        else
            c /= 3;           /* guaranteed one of a,b,c divisible by 3 */

        /* multiply: use 128‑bit to avoid intermediate overflow */
        __uint128_t prod = (__uint128_t)a * b * c;
        uint64_t t = (uint64_t)prod;  /* t fits in 64‑bit because t ≤ n ≤ 2^64−1 */

        if (t > n)
            break;                /* no larger tetrahedral number divides n */

        if (n % t == 0)
            sum += (seq_t)t;
    }
    return sum;
}
