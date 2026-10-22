/*
 * A399211  Number of solutions to x^2 + 8*y^2 = n in positive integers x and
 *          y.
 * https://oeis.org/A399211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8503, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counting solutions of x^2 + 8*y^2 = n in positive integers
   (OEIS A399211).  Brute‑force iterate over y, check if the remaining
   part is a perfect square.  The algorithm is O(√n). */

/* Integer square root for uint64_t, implemented without external
   helpers.  Returns floor(√x). */
static inline idx_t isqrt_u64(uint64_t x)
{
    idx_t lo = 0, hi = (idx_t)x;            /* hi fits into idx_t for 64‑bit x */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = (uint64_t)mid * (uint64_t)mid;
        if (sq == x) return mid;
        if (sq < x) lo = mid + 1;
        else        hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t sn = (seq_t)n;        /* promote index to seq_t for computations */

    for (idx_t y = 1;; ++y) {
        seq_t y2 = (seq_t)y * (seq_t)y;   /* y^2 fits into idx_t */
        seq_t t  = 8 * y2;                /* 8*y^2 */
        if (t > sn) break;                /* no more y possible */
        seq_t x2 = sn - t;                /* remaining part for x^2 */
        if (x2 == 0) continue;            /* x must be positive */

        idx_t x = isqrt_u64((uint64_t)x2); /* integer sqrt of x2 */
        seq_t sq = (seq_t)x * (seq_t)x;
        if (sq == x2) count++;             /* found a solution */
    }

    return count;
}
