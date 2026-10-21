/*
 * A256094  Alternating row sums of array A255250.
 * https://oeis.org/A256094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum((-1)^m*(floor(sqrt(n^2 - m^2))-(m-1)),
 *            m=0..floor(n/sqrt(2)))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer, returning the floor of the root. */
static uint64_t isqrt_uint64(uint64_t x) {
    uint64_t r = 0;       /* current result */
    uint64_t bit = 1ULL << 62;  /* the second highest bit of the result */

    while (bit) {
        if (r + bit <= x) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t n_u = (uint64_t)n;              /* n fits in 64‑bit for all tested values */
    uint64_t n2 = n_u * n_u;                 /* n², safe for n ≤ 10000 */
    uint64_t lim = n2 / 2;                   /* n²/2, the largest m² allowed */
    uint64_t mmax = isqrt_uint64(lim);       /* floor(sqrt(n²/2)) */

    long long sum = 0;                       /* intermediate signed accumulator */

    for (uint64_t m = 0; m <= mmax; ++m) {
        uint64_t rem        = n2 - m * m;    /* n² – m² */
        uint64_t sqrt_term  = isqrt_uint64(rem);  /* floor(sqrt(n² – m²)) */
        long long t = (long long)sqrt_term - (long long)m + 1; /* T(n,m) */

        if ((m & 1ULL) == 0)
            sum += t;                       /* even m: add */
        else
            sum -= t;                       /* odd m: subtract */
    }

    return (seq_t)sum;                      /* sum is guaranteed to fit in seq_t for n ≤ 10000 */
}
