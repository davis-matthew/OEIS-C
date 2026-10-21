/*
 * A292642  Rank of (5-r)*n when all the numbers (5-r)*j and (5+r)*k, where r
 *          = sqrt(2), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n * (5 - sqrt(2)) / (5 + sqrt(2)))
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root of a 64‑bit unsigned integer */
static inline uint64_t int_sqrt_u64(uint64_t x) {
    uint64_t lo = 0, hi = (uint64_t)1 << 32; /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)   /* mid*mid > x, using division to avoid overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Compute a(n) = n + floor(n / α), where α = (5+√2)/(5-√2) */
    /* α = (27+10√2)/23, so 1/α = (27-10√2)/23   */
    /* Hence floor(n/α) = floor(n * (27-10√2) / 23) */

    uint64_t n64 = (uint64_t)n;          /* idx_t is uint64_t */

    /* n * (27-10√2) / 23  →  we compute floor((27n - 10n√2)/23) */
    uint64_t p = 27ULL * n64;            /* 27n   */
    uint64_t q = 10ULL * n64;            /* 10n   */

    /* Compute floor(q * sqrt(2))  = floor(sqrt(2*q*q)) */
    uint64_t sq = 2ULL * q * q;          /* 2*q^2  (fits 64 bits for n up to 10^4) */
    uint64_t floor_q_sqrt2 = int_sqrt_u64(sq);

    /* UB = p - floor(q * sqrt(2))  = 27n - floor(10n*sqrt(2))  > 0 */
    uint64_t UB = p - floor_q_sqrt2;

    /* floor( UB / 23 ) ; adjustment if UB is a multiple of 23 */
    uint64_t f = UB / 23ULL;
    if (UB % 23ULL == 0)
        f--;

    /* Result: n + f  */
    return (seq_t)n64 + (seq_t)f;
}
