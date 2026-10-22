/*
 * A362783  Square array A(n,k) = (n^(2*k + 1) + 1)/(n + 1), n >= 0, k >= 0,
 *          read by antidiagonals.
 * https://oeis.org/A362783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(row,col) where row and col are obtained by * reading
 *            the square array A(n,k) = (n^(2*k+1)+1)/(n+1) * by
 *            antidiagonals. This implementation uses a * combinatorial
 *            identity and avoids large intermediate values
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 82, u64 n <= 194,
 *            u128 n <= 471, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 128‑bit unsigned value (floor) */
static uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1ULL << 62;          /* 2^62 * 2^62 = 2^124 <= 2^128 */
    while (bit > 0) {
        uint64_t test = r | bit;
        if ((unsigned __int128)test * test <= x)
            r = test;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    unsigned __int128 tmp = (unsigned __int128)n * 8 + 1;     /* 8*n+1 */
    uint64_t sq   = isqrt_u128(tmp);
    uint64_t d    = (sq - 1) / 2;

    /* triangular number T_d = d*(d+1)/2 */
    unsigned __int128 t12 = (unsigned __int128)d * (d + 1) / 2;
    uint64_t Td = (uint64_t)t12;

    uint64_t t  = n - Td;        /* offset on the diagonal */
    uint64_t row = t;            /* first argument of A(n,k) */
    uint64_t col = d - t;        /* second argument of A(n,k) */

    /* Compute A(row,col) = sum_{j=0}^{2*col} (-row)^j
     * using separate positive and negative accumulations to avoid
     * intermediate underflow. */
    uint64_t limit = 2 * col;
    seq_t pos = 0;
    seq_t neg = 0;
    seq_t power = 1;             /* row^j */

    for (uint64_t j = 0; j <= limit; ++j) {
        if ((j & 1ULL) == 0)
            pos += power;
        else
            neg += power;

        if (j < limit)
            power = power * (seq_t)row;
    }

    return pos - neg;            /* pos >= neg for all cases */
}
