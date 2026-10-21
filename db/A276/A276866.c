/*
 * A276866  First differences of the Beatty sequence A004976 for 2 + sqrt(5).
 * https://oeis.org/A276866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(2+√5)) - floor((n-1)*(2+√5))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                = 2 + ( floor(n*√5) - floor((n-1)*√5) )  */

static inline unsigned long long isqrt_ull(unsigned long long x)
{
    /* integer square root: largest r with r*r <= x */
    unsigned long long lo = 0, hi = x, mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if (mid == 0) { lo = 1; continue; }
        if (mid <= x / mid) {          /* mid*mid <= x   */
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, safety only */

    unsigned long long nn = (unsigned long long)n;
    unsigned long long sq1 = 5ULL * nn * nn;
    unsigned long long f1 = isqrt_ull(sq1);          /* floor(n*√5)   */

    unsigned long long mm = nn - 1;
    unsigned long long sq2 = 5ULL * mm * mm;
    unsigned long long f2 = isqrt_ull(sq2);          /* floor((n-1)*√5) */

    unsigned long long delta = f1 - f2;             /* 2 or 3         */
    return (seq_t)(2 + delta);                      /* 4 or 5         */
}
