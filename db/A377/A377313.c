/*
 * A377313  Numbers that are not hexateron (5-simplex) numbers.
 * https://oeis.org/A377313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+m if n+m>binomial(m+4,5); a(n) = n+m-2 if
 *            n+m-2<binomial(m+3,5); otherwise a(n)=n+m-1, where m =
 *            floor((120*n)^(1/5))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial(n,5) as 128‑bit integer; return 0 if n<5. */
static inline __int128 binom5_u128(idx_t n)
{
    if (n < 5) return 0;
    __int128 r = n;
    r *= (__int128)(n - 1);
    r *= (__int128)(n - 2);
    r *= (__int128)(n - 3);
    r *= (__int128)(n - 4);
    return r / 120;  /* 5! = 120 */
}

/* Compute x^5 as 128‑bit integer (x is <= ~5×10⁴). */
static inline __int128 pow5_u128(idx_t x)
{
    __int128 r = x;
    r *= (__int128)x; /* x² */
    r *= (__int128)x; /* x³ */
    r *= (__int128)x; /* x⁴ */
    r *= (__int128)x; /* x⁵ */
    return r;
}

/* Return floor((k)^(1/5)) for a 128‑bit integer k. */
static inline idx_t floor_fifth_root_u128(__int128 k)
{
    idx_t low = 0, high = 1;

    /* Grow high until high^5 > k */
    while (pow5_u128(high) <= k) {
        low = high;
        high <<= 1;
        if (high == 0) { /* overflow guard */
            break;
        }
    }

    /* Binary search in [low, high] */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (pow5_u128(mid) <= k)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Main routine for the sequence */
seq_t a(idx_t n)
{
    /* m = floor((120*n)^(1/5)) */
    __int128 prod = (__int128)120 * n;
    idx_t m = floor_fifth_root_u128(prod);

    __int128 candidate = (__int128)n + m;
    __int128 bin1   = binom5_u128(m + 4);   /* binomial(m+4,5)   */
    __int128 bin2   = binom5_u128(m + 3);   /* binomial(m+3,5)   */

    if (candidate > bin1)
        return (seq_t)candidate;

    if ((candidate - 2) < bin2)
        return (seq_t)(candidate - 2);

    return (seq_t)(candidate - 1);
}
