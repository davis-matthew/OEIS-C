/*
 * A330190  Symmetric matrix read by antidiagonals: f(i,j) = 1 + gcd(f(i-1,j),
 *          f(i,j-1)), where f(1,j) and f(i,1) are 1.
 * https://oeis.org/A330190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = value of the matrix f(i,j) read by antidiagonals where
 *            f(i,j) = 1 + gcd(f(i-1,j), f(i,j-1)), and f(1,*) = f(*,1) = 1.
 *            Indexing starts at 1
 * Range:     u8 n <= 9082, u16 n <= 9082, u32 n <= 9082, u64 n <= 9082,
 *            u128 n <= 9082, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_DIM 200   /* sufficient for n up to about 10^4 */

static seq_t f[MAX_DIM][MAX_DIM];
static bool initialized = false;

static void ensure_matrix(void)
{
    if (initialized) return;
    for (idx_t i = 0; i < MAX_DIM; ++i) {
        for (idx_t j = 0; j < MAX_DIM; ++j) {
            if (i == 0 || j == 0)
                f[i][j] = (seq_t)1;
            else
                f[i][j] = (seq_t)1 + gcd(f[i-1][j], f[i][j-1]);
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range, return 0 */
    ensure_matrix();

    /* Find the antidiagonal t such that (t-1)*t/2 >= n  (t >= 2)   */
    idx_t t = 2;
    while ((t-1) * t / 2 < n) ++t;

    /* Offset within this antidiagonal, 1-based   */
    idx_t prev = (t-1) * (t-2) / 2;
    idx_t offset = n - prev;          /* 1 <= offset <= t-1   */

    /* Convert to 0‑based indices for the stored array      */
    idx_t i = offset - 1;             /* row index            */
    idx_t j = t - offset - 1;         /* column index         */
    return f[i][j];
}
