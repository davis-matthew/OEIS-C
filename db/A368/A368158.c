/*
 * A368158  Irregular triangular array T, read by rows: T(n,k) = number of
 *          sums |x-y| + |y-z| = k, where x,y,z are in {1,2,...,n} and x <= y.
 * https://oeis.org/A368158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) yields the n-th term of OEIS A368158. For each index n
 *            (offset = 1), we determine the row m = ⌊√n⌋ and the position k =
 *            n − m² (0‑based inside the row). The value is the number of
 *            triples (x,y,z) with 1≤x≤y≤N, 1≤z≤N, where N = m + 1, such that
 *            |x‑y| + |y‑z| = k. A closed‑form combinatorial e...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* integer square root: floor(sqrt(x)) for 0 ≤ x < 2^64 */
static idx_t isqrt_u64(idx_t x)
{
    if (x <= 1)
        return x;
    idx_t r = (idx_t)sqrt((long double)x);
    while ((unsigned __int128)(r + 1) * (r + 1) <= x)
        ++r;
    while ((unsigned __int128)r * r > x)
        --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined in the OEIS, guard */

    /* row index (m) and position (k) inside the row */
    idx_t m = isqrt_u64(n);          /* floor(√n)  → 1 ≤ m */
    idx_t k = n - m * m;             /* 0 ≤ k ≤ 2m      */

    /* N = maximum element value in the original definition (N = m+1) */
    idx_t N = m + 1;

    /* use a 128‑bit accumulator for intermediate results */
    unsigned __int128 sum = 0;

    /* Case 1: triples with z ≥ y (k = z − x) */
    if (k <= N - 1) {
        sum += (unsigned __int128)(N - k) * (k + 1);
    }

    /* Case 2: triples with z < y */
    if (k >= 1) {
        /* Range 1: y such that k ≤ y−1  →  contribution = k for each y */
        idx_t y1_start = k + 1;
        if (y1_start < 2)
            y1_start = 2;
        if (y1_start <= N) {
            idx_t cnt1 = N - y1_start + 1;
            sum += (unsigned __int128)k * cnt1;
        }

        /* Range 2: y such that y ≤ k ≤ 2y−2  →  contribution = 2y−k−1 */
        idx_t lower2 = (k + 3) / 2;   /* ceil((k+2)/2) */
        if (lower2 < 2)
            lower2 = 2;
        idx_t upper2 = (k < N) ? k : N;

        if (lower2 <= upper2) {
            idx_t cnt2 = upper2 - lower2 + 1;
            unsigned __int128 sum_y = (unsigned __int128)(lower2 + upper2) * cnt2 / 2;
            sum += 2 * sum_y - (unsigned __int128)(k + 1) * cnt2;
        }
    }

    return (seq_t)sum;
}
