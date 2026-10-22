/*
 * A370832  Triangle read by rows: T(n,k) gives the number of parking
 *          functions of size n with k lucky cars. 0 <= k <= n.
 * https://oeis.org/A370832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the number of parking functions of size n with k lucky
 *            cars, * read row by row as a concatenation of the rows of the
 *            triangle T(n,k). * The sequence a(n) is defined as follows: *
 *            a(0) = 1 * For n > 0: * Let m be the largest integer with
 *            m(m+1)/2 ≤ n. * Let k = n - m(m+1)/2. * If k == 0 →...
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 70, u64 n <= 160,
 *            u128 n <= 418, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Helper: compute T(m,k) using the generating function product.
 * m ≥ 1, 1 ≤ k ≤ m.  The result fits in seq_t.
 */
static seq_t compute_T(idx_t m, idx_t k) {
    /* Polynomal array: indices 0..m.  poly[i] will hold coefficient of x^i. */
    seq_t *poly = (seq_t *)calloc((size_t)(m + 1), sizeof(seq_t));
    if (!poly) return (seq_t)0;      /* out‑of‑memory guard; silently returns 0 */

    /* Start with the initial factor x.  poly[0] stays 0. */
    poly[1] = (seq_t)1;
    idx_t deg = 1;          /* current maximal degree */

    for (idx_t j = 2; j <= m; ++j) {
        seq_t a = (seq_t)(m + 1 - j);   /* constant term of (m+1 + j·(x-1)) */
        seq_t b = (seq_t)j;             /* coefficient of x in the factor */

        /* multiply poly by (a + b·x), updating in reverse order */
        for (idx_t i = deg; i > 0; --i) {
            seq_t old = poly[i];
            poly[i]     = old * a;          /* old * a  → new coefficient at x^i   */
            poly[i + 1] += old * b;        /* old * b  → new coefficient at x^(i+1) */
        }
        /* poly[0] remains 0; it can be multiplied by a but stays 0  */
        /* increase the maximal degree by one */
        ++deg;
    }

    seq_t result = poly[k];
    free(poly);
    return result;
}

/* Main sequence function a(idx_t n) */
seq_t a(idx_t n) {
    /* Base case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Find the largest m such that m(m+1)/2 ≤ n.
     * Use an initial bound from the quadratic equation.
     */
    idx_t m;
    {
        /* Use double precision to obtain a rough upper bound. */
        double d = sqrt((long double)2.0L * (long double)n);
        idx_t high = (idx_t)(d) + 2;   /* safety margin */
        idx_t low  = 0;

        /* Exact binary search in [low, high] */
        while (low < high) {
            idx_t mid = (low + high + 1) >> 1;
            __int128 tmid = (__int128)mid * (mid + 1) / 2;
            if (tmid <= n)
                low = mid;
            else
                high = mid - 1;
        }
        m = low;
    }

    /* Compute triangular number T_m */
    idx_t t_m = (idx_t)((__int128)m * (m + 1) / 2);
    idx_t k = n - t_m;

    /* If k == 0, the index falls at a triangular number → a(n) = 0 */
    if (k == 0)
        return (seq_t)0;

    /* Otherwise, a(n) = T(m, k) */
    return compute_T(m, k);
}
