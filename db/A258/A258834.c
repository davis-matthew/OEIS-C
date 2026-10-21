/*
 * A258834  Nonhomogeneous Beatty sequence: a(n) = ceiling((n - 1/4)*(2 +
 *          sqrt(2))).
 * https://oeis.org/A258834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n - 1/4)*(2 + sqrt(2))) = floor((n - 1/4)*(2 +
 *            sqrt(2)) + 1)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer auxiliary: floor(sqrt(x)) for unsigned __int128 x, return uint64_t */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low  = 0;
    uint64_t high = UINT64_MAX;          /* sqrt(2^128-1) fits in 64 bits */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                         /* floor of the square root */
}

seq_t a(idx_t n)
{
    /* Promote n to an unsigned __int128 to avoid overflow in intermediate steps */
    unsigned __int128 n128 = n;

    /* a4 = 4*n  (may exceed seq_t, but fits in 128 bits for all practical n) */
    unsigned __int128 a4 = n128 << 2;      /* 4 * n128 */

    /* B² = (4n - 1)² = a4² - 2*a4 + 1 */
    unsigned __int128 Bsq = a4 * a4 - (unsigned __int128)2 * a4 + 1;

    /* 2*B² – value whose integer square root is needed */
    unsigned __int128 val = Bsq << 1;      /* 2 * Bsq */

    /* S = floor(sqrt(2*B²))  – fits in uint64_t because sqrt(2*B²) <= sqrt(2)*|4n-1| */
    uint64_t S = isqrt_uint128(val);

    /* A = 8*n + 2  (from (8n+2) + (4n-1)*sqrt(2) + 4 numerator) */
    unsigned __int128 A = (n128 << 3) + 2;

    /* Numerator after adding S */
    unsigned __int128 numerator = A + (unsigned __int128)S;

    /* Result: floor((A + S) / 4) */
    unsigned __int128 result = numerator >> 2;   /* equivalent to /4 for non‑negative integers */

    /* Cast back to seq_t – truncation is acceptable when the true value fits into seq_t */
    return (seq_t)result;
}
