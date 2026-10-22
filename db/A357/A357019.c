/*
 * A357019  a(n) is the largest possible x in n = x^2 - x*y + y^2 with
 *          integers x > y >= 0, or 0 if n cannot be expressed in this form.
 * https://oeis.org/A357019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest integer x such that n = x^2 - x*y + y^2 for
 *            some integer y with 0 ≤ y < x
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for unsigned __int128.  The result is floor(sqrt(v)). */
static unsigned __int128 isqrt_uint128(unsigned __int128 v)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1) << 35;  /* 2^35 > sqrt(2^66) */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        unsigned __int128 sq  = mid * mid;      /* fits in 128 bits */
        if (sq <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Upper bound for x: floor(sqrt(4n/3)) */
    unsigned __int128 val = ((unsigned __int128)4 * (unsigned __int128)n) / 3;
    seq_t ub = (seq_t)isqrt_uint128(val);

    for (seq_t x = ub; x > 0; --x) {
        unsigned long long xl = (unsigned long long)x;          /* x fits in 64 bits */
        unsigned __int128 a128 = (unsigned __int128)4 * (unsigned __int128)n;
        unsigned __int128 b128 = (unsigned __int128)3 * xl * xl;

        if (a128 < b128)            /* D would be negative; skip */
            continue;

        unsigned __int128 D = a128 - b128;
        unsigned __int128 sqrtD = isqrt_uint128(D);

        if (sqrtD * sqrtD != D)     /* D not a perfect square */
            continue;

        /* Parity condition: (x ± sqrtD) must be even.  Using the minus sign gives y ≥ 0. */
        if ((x & 1) != (sqrtD & 1))
            continue;

        return x;                   /* first (i.e. largest) valid x encountered */
    }
    return 0;                        /* no representation found */
}
