/*
 * A288936  Fixed point of the mapping 00->0010, 01->011, 10->011, starting
 *          with 00.
 * https://oeis.org/A288936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if (n-6) equals k(k+3)/2 for some integer k≥1;
 *            otherwise 1. The first six terms are: 0,0,1,0,0,1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) / 2;
        if (mid <= n / mid)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Handle the first six entries directly. */
    switch (n)
    {
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)1;
        case 4: return (seq_t)0;
        case 5: return (seq_t)0;
        case 6: return (seq_t)1;
        default:
            break;
    }

    /* For n ≥ 7, m = n – 6. */
    idx_t m = n - 6;

    /* Compute the discriminant D = 9 + 8*m. */
    idx_t D = (idx_t)9 + (idx_t)8 * m;

    /* Integer square root of D. */
    idx_t sqrtD = isqrt_u64(D);

    /* Check if D is a perfect square. */
    if (sqrtD * sqrtD != D)
        return (seq_t)1;

    /* sqrtD must be ≥ 3 and (sqrtD – 3) even. */
    if (sqrtD < 3 || ((sqrtD - 3) & 1u) != 0u)
        return (seq_t)1;

    idx_t k = (sqrtD - 3) >> 1;

    /* Verify that k(k+3)/2 equals m. */
    if ((k * (k + 3)) / 2 == m)
        return (seq_t)0;

    return (seq_t)1;
}
