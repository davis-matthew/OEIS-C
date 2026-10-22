/*
 * A360853  Array read by antidiagonals: T(m,n) is the number of induced
 *          cycles in the rook graph K_m X K_n.
 * https://oeis.org/A360853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of induced cycles in the rook graph * K_m × K_n,
 *            where (m,n) are the coordinates read by * antidiagonals. T(m,n)
 *            = m*C(n,3)+n*C(m,3) + * Σ_{p=2}^{min(m,n)} C(m,p) * C(n,p) * p *
 *            ((p-1)!)² / 2
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 143, u64 n <= 337,
 *            u128 n <= 881, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t idx_t;  // provided by the build
typedef unsigned __int128 u128;  // internal 128‑bit type for safety

/* ---------- utility functions ---------- */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

static seq_t fact_upto(idx_t n)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= n; ++i) f *= (seq_t)i;
    return f;
}

/* Find smallest s such that (s-1)*s/2 >= n */
static idx_t diag_start(idx_t n)
{
    idx_t low = 2, high = 2;
    while ((u128)(high - 1) * high / 2 < (u128)n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((u128)(mid - 1) * mid / 2 >= (u128)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Determine coordinates (m,i) and (n,j) from antidiagonal index n */
    idx_t s = diag_start(n);                     /* s = m + n              */
    idx_t prev = (u128)(s - 1) * (s - 2) / 2;    /* indices before this diag   */
    idx_t offset = n - prev - 1;                 /* position in this diag     */
    idx_t m = 1 + offset;                        /* row index                 */
    idx_t nn = s - m;                            /* column index              */

    /* Count triangles: m*C(nn,3)+nn*C(m,3) */
    seq_t total = 0;
    total += (seq_t)m * binom(nn, 3) + (seq_t)nn * binom(m, 3);

    /* Count cycles of length 2p for p >= 2 */
    idx_t pmax = (m < nn) ? m : nn;
    for (idx_t p = 2; p <= pmax; ++p) {
        seq_t rows = binom(m, p);
        seq_t cols = binom(nn, p);
        seq_t fp   = fact_upto(p - 1);
        seq_t cycles_per_p = (seq_t)p * fp * fp / 2;   /* p*((p-1)!)²/2 */
        total += rows * cols * cycles_per_p;
    }

    return total;
}
