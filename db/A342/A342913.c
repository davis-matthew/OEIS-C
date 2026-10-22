/*
 * A342913  Pairwise listing of the partitions of 2k into two parts, (s,t),
 *          with 0 < t <= s ordered by decreasing values of s and where k =
 *          1,2,... .
 * https://oeis.org/A342913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k - (k^2 + k - m)*(-1)^n / 2 where k = round(sqrt(m)) and
 *            m = 2*floor((n+1-(-1)^n)/2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root (floor) for 64‑bit unsigned values.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1U << 32);          /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * mid;
        if (prod <= n) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;   /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* Compute temporary sign factor (-1)^n */
    int sign = (n & 1) ? -1 : 1;

    /* Compute m = 2 * floor((n + 1 - (-1)^n) / 2) */
    int64_t sign1 = sign;                     /* (-1)^n */
    int64_t temp  = (int64_t)n + 1 - sign1;   /* n + 1 - (-1)^n */
    int64_t half  = temp / 2;                 /* floor division, positive */
    idx_t m = (idx_t)(2 * half);

    /* Compute rounded sqrt: k = round(sqrt(m)) */
    idx_t r  = floor_sqrt_u64(m);              /* floor(sqrt(m))       */
    __int128 rr  = (__int128)r * r;           /* r^2                  */
    __int128 r1r1= (__int128)(r + 1) * (r + 1);/* (r+1)^2              */
    __int128 diff1 = (__int128)m - rr;         /* m - r^2              */
    __int128 diff2 = r1r1 - (__int128)m;       /* (r+1)^2 - m          */
    idx_t k = (diff2 < diff1) ? (r + 1) : r;   /* round(sqrt(m))       */

    /* Compute a(n) */
    __int128 kk  = (__int128)k;                    /* for safety   */
    __int128 term = kk * kk + kk - (__int128)m;    /* k^2 + k - m  */
    term *= sign;                                   /* multiply by (-1)^n */
    __int128 half_term = term / 2;                  /* exact division   */
    __int128 a_val = kk - half_term;                /* final value      */

    return (seq_t)a_val;
}
