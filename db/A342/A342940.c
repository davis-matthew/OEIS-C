/*
 * A342940  Triangle read by rows: T(n, k) is the Skolem number of the
 *          parallelogram graph P_{n, k}, with 1 < k <= n.
 * https://oeis.org/A342940
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A342940
 *  Sequence: a(n) = T(r, k) where
 *      T(r, k) = k*r - 2*k - r + 4          (Skolem number of P_{r,k})
 *  The index n (offset 2) runs over row‑major order of the triangular table
 *      for 2 ≤ k ≤ r,  r ≥ 2.
 *
 *  For a given n ≥ 2:
 *      let g = n-1                                 // 0–based index into the triangle
 *      find the smallest r such that g ≤ r(r-1)/2
 *      k = g - (r-2)(r-1)/2 + 1       // column index (starting from 2)
 *
 *  This implementation computes the required r via integer arithmetic
 *  (no floating point), then evaluates T(r,k) exactly in seq_t.       */

#include <stdint.h>

/* integer square‑root (floor) using only integer arithmetic */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0)
        {
            low = 1;
            continue;
        }
        idx_t div = n / mid;
        if (mid <= div)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Main function computing a(n) from OEIS A342940 */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* undefined for the offset; value not used */

    idx_t g = n - 1;                    /* 0‑based position in flattened triangle */

    /* find smallest r such that g ≤ r(r-1)/2   (r ≥ 2)   */
    idx_t r = 2;
    /* using integer formula: r = ceil((1 + sqrt(1+8g))/2)  */
    idx_t disc = 1 + 8 * g;
    idx_t sq = isqrt_u64(disc);              /* floor sqrt of disc       */
    r = (1 + sq) >> 1;                       /* (1+sqrt)/2, truncated   */
    /* adjust if not large enough */
    if (r * (r - 1) / 2 < g)
        ++r;

    /* compute previous prefix sum H(r-1) = (r-2)(r-1)/2 */
    idx_t prev = (r - 2) * (r - 1) / 2;
    idx_t k = g - prev + 1;                  /* column (k ≥ 2)           */

    /* evaluate T(r, k) = k*r - 2*k - r + 4 */
    seq_t value = (seq_t)k * r - (seq_t)2 * k - (seq_t)r + (seq_t)4;
    return value;
}
