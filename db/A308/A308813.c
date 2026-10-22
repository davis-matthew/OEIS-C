/*
 * A308813  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) is Sum_{d|n} k^(d-1).
 * https://oeis.org/A308813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Uses the definition A(n,k) = Σ_{d|n} k^(d-1) read by
 *            antidiagonals. a(m) = A(r,k) where (r,k) is obtained from m by
 *            moving along the antidiagonals in the square array
 * Range:     u8 n <= 40, u16 n <= 85, u32 n <= 201, u64 n <= 483,
 *            u128 n <= 1208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* required by the build system */

/* Computes k^(e) for unsigned integer or arbitrary-precision seq_t. */
static seq_t ipow(seq_t base, unsigned e)
{
    seq_t res = 1;
    while (e > 0) {
        res *= base;
        --e;
    }
    return res;
}

/* Compute A(r,k) = sum_{d|r} k^(d-1). */
static seq_t A_value(idx_t r, idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d == 0) {
            sum += ipow((seq_t)k, (unsigned)(d - 1));
            idx_t d2 = r / d;
            if (d2 != d) {
                sum += ipow((seq_t)k, (unsigned)(d2 - 1));
            }
        }
    }
    return sum;
}

/* Convert sequence index m (1‑based) to (row r, column k) in the square array. */
static void index_to_coord(idx_t m, idx_t *row, idx_t *col)
{
    /* Find diagonal d such that (d+1)(d+2)/2 >= m */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < m) ++d;

    idx_t terms_before = d * (d + 1) / 2;
    idx_t offset = m - terms_before - 1; /* 0‑based offset in diagonal */

    *row = offset + 1;                /* rows start at 1 */
    *col = d + 1 - *row;              /* columns start at 0 */
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but not expected */
    idx_t r, k;
    index_to_coord(n, &r, &k);
    return A_value(r, k);
}
