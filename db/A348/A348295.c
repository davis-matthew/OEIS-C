/*
 * A348295  a(n) = Sum_{k=1..n} (-1)^(floor(k*(sqrt(2)-1))).
 * https://oeis.org/A348295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(floor(k*(sqrt(2)-1))). Relation:
 *            floor(k*(sqrt(2)-1)) = floor(k*sqrt(2)) - k. Sign of the k-th
 *            term = (-1)^{k + floor(k*sqrt(2))}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 128‑bit values: floor(sqrt(x)) */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 val = (unsigned __int128)k * k * 2;   /* 2*k^2   */
        unsigned __int128 sq  = isqrt128(val);                  /* floor(k*sqrt(2)) */
        uint64_t parity = (uint64_t)(sq & 1);                  /* parity of floor(k*sqrt2) */
        uint64_t k_par  = (uint64_t)(k & 1);                    /* parity of k          */
        /* sign = +1 if (k_par ^ parity) == 0, else -1          */
        if ((k_par ^ parity) == 0)
            sum += (seq_t)1;
        else
            sum -= (seq_t)1;
    }
    return sum;
}
