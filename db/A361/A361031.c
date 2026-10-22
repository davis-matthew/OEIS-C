/*
 * A361031  a(n) = (3^3)*(1*2*4*5*7*8*10*11)*(3*n)!/(n!*(n+4)!^2).
 * https://oeis.org/A361031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^3)*(1*2*4*5*7*8*10*11)*(3*n)!/(n!*(n+4)!^2) /*
 *            Recurrence: a(0) = 11550 * a(n) =
 *            3*(3*n-1)*(3*n-2)*a(n-1)/((n+4)^2) for n >= 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 11550;

    seq_t val = 11550;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* numerator factors: 3, 3*i-1, 3*i-2   (unsigned 64‑bit fits) */
        unsigned long long f1 = 3ULL;
        unsigned long long f2 = 3ULL * i - 1ULL;
        unsigned long long f3 = 3ULL * i - 2ULL;

        /* denominator: (i+4)^2   fits in 32‑bit for i <= 10000 */
        unsigned long long d  = (unsigned long long)(i + 4ULL);
        unsigned long long denom = d * d;

        /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds.
           The result is guaranteed to be integer. */
        unsigned __int128 prod = (unsigned __int128)val * f1;
        prod = prod * f2;
        prod = prod * f3;
        prod = prod / denom;

        val = (seq_t)prod;
    }
    return val;
}
