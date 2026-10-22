/*
 * A332934  Sum of round(d^(3/2)) where d runs through the divisors of n.
 * https://oeis.org/A332934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} round(d^(3/2))
 * Range:     u8 n <= 29, u16 n <= 1079, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for unsigned 64‑bit values: floor(sqrt(x)) */
static inline idx_t isqrt_u64(idx_t x)
{
    /* binary search */
    if (x < 2) return x;
    idx_t low = 0;
    idx_t high = (x >> 1) + 1;
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Round d^(3/2) = nearest integer to sqrt(d^3) */
static inline idx_t rounded_pow32(idx_t d)
{
    /* d^3 fits in 64‑bit for all n in the problem range */
    idx_t cube = d * d * d;
    idx_t r = isqrt_u64(cube);
    idx_t r2 = r * r;
    idx_t diff1 = cube - r2;
    idx_t r1 = r + 1;
    idx_t r1sq = r1 * r1;
    idx_t diff2 = r1sq - cube;
    return (diff1 <= diff2) ? r : r1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            sum += (seq_t)rounded_pow32(i);
            idx_t d2 = n / i;
            if (d2 != i)
                sum += (seq_t)rounded_pow32(d2);
        }
    }
    return sum;
}
