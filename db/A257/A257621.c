/*
 * A257621  Triangle read by rows: T(n, k) = t(n-k, k), where t(0,0) = 1,
 *          t(n,m) = 0 if n < 0 or m < 0, else t(n,m) = f(m)*t(n-1,m) +
 *          f(n)*t(n,m-1), and f(n) = 4*n + 3.
 * https://oeis.org/A257621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) * * The triangle T is defined by * T(r,c) =
 *            t(r-c,c) * * with * t(0,0) = 1 * t(n,m) = 0 if n < 0 or m < 0 *
 *            t(n,m) = (4*m+3)*t(n-1,m) + (4*n+3)*t(n,m-1) otherwise * * The
 *            mapping from the one‑dimensional index n to the row r and column
 *            * c of T is the usual triangular indexing: * r...
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 47, u64 n <= 110, u128 n <= 333,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----- Integer square root for unsigned __int128 ----- */
static idx_t
isqrt_u128(unsigned __int128 x)
{
    /* Upper bound large enough for all 64‑bit inputs: sqrt(8*n+1) <= 2^33 */
    idx_t lo = 0;
    idx_t hi = (idx_t)((unsigned __int128)1 << 34);
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* ----- Main sequence function ----- */
seq_t
a(idx_t n)
{
    /* --- Determine the row r and column c of T --- */
    unsigned __int128 t8n = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(t8n);               /* floor(sqrt(8*n+1)) */
    idx_t r = (s - 1) / 2;                   /* triangle index (row) */
    unsigned __int128 dtri = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = (idx_t)(n - dtri);             /* column index */
    if (c > r) c = r;                        /* safety for overflow */

    /* --- Translate to t coordinates: n' = r-c, m' = c --- */
    idx_t np = r - c;   /* >= 0 */
    idx_t mp = c;       /* >= 0 */

    /* --- Build the sub‑triangle of t up to (np, mp) --- */
    size_t rows = (size_t)np + 1;
    size_t cols = (size_t)mp + 1;
    size_t total = rows * cols;

    seq_t *arr = (seq_t *)calloc(total, sizeof(seq_t));
    if (!arr) return (seq_t)0;   /* allocation failure – unlikely */

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            size_t idx = i * cols + j;
            if (i == 0 && j == 0) {
                arr[idx] = (seq_t)1;
                continue;
            }
            seq_t val = 0;
            if (i > 0) {
                seq_t term1 = (seq_t)(4 * j + 3) * arr[(i - 1) * cols + j];
                val += term1;
            }
            if (j > 0) {
                seq_t term2 = (seq_t)(4 * i + 3) * arr[i * cols + (j - 1)];
                val += term2;
            }
            arr[idx] = val;
        }
    }

    /* --- Result is t(np, mp) --- */
    seq_t ans = arr[rows * cols - 1];   /* index np*cols + mp */

    free(arr);
    return ans;
}
