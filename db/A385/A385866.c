/*
 * A385866  Triangle read by rows where T(n,k), for 1 <= k < n, is the row
 *          number where (n-k)^2 occurs in an n X n grid filled rowwise with
 *          the numbers 1 to n^2.
 * https://oeis.org/A385866
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Let m = n – 2 (zero‑based index in the flattened triangle). Let
 *            r = ⌈(1 + √(1 + 8m)) / 2⌉ + 1 (row number containing the m‑th
 *            term). Let k = m – (r–2)(r–1)/2 + 1 (column index, 1 ≤ k < r).
 *            Then a(n) = ⌊((r – k)² – 1)/r⌋ + 1. This is equivalent to the
 *            OEIS definition of the triangle read by rows
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* n must be ≥ 2; behaviour for smaller n is undefined (offset 2). */
    idx_t m = n - 2;                /* zero‑based position in the flattened triangle */

    /* Approximate r using the quadratic formula and adjust to the exact value. */
    long double sq = sqrtl((long double)(1ULL + 8ULL * m));
    idx_t s = (idx_t)sq;               /* floor of the sqrt */
    idx_t r = (1 + s) / 2 + 1;         /* initial estimate of the row number */

    /* Correct r so that (r-1)*r/2 > m and (r-2)*(r-1)/2 ≤ m. */
    idx_t sum = (r - 1) * r / 2;
    while (sum <= m) {
        ++r;
        sum = (r - 1) * r / 2;
    }
    while (r > 2) {
        idx_t prev = (r - 2) * (r - 1) / 2;
        if (prev > m) {
            --r;
        } else {
            break;
        }
    }

    /* Position inside the found row. */
    idx_t pre   = (r - 2) * (r - 1) / 2;
    idx_t j     = m - pre;               /* 0‑based offset within row */
    idx_t k     = j + 1;                 /* column index (1 ≤ k < r) */

    /* Compute the triangle entry T(r, k). */
    seq_t delta = (seq_t)(r - k);
    seq_t res   = ((delta * delta - (seq_t)1) / (seq_t)r) + (seq_t)1;
    return res;
}
