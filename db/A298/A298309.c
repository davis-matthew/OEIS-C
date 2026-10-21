/*
 * A298309  Triangle read by rows: T(n,m) = Sum_{i=0..n+1}
 *          C(n-i+1,i-1)*C(n-i+1,i)*C(n-i+1,m-i+1).
 * https://oeis.org/A298309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(row,col) where the triangle T is 
 *   T(n,m) = Σ_{i=0}^{n+1} C(n-i+1,i-1)·C(n-i+1,i)·C(n-i+1,m-i+1).
 * The index n is flattened row‑wise, so 
 *   row = ⌊(√(8n+1)-1)/2⌋ , 
 *   col = n - row(row+1)/2.
 * The function below implements this directly using only integer arithmetic.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Integer square root for unsigned 64‑bit values (floor).  Works for
 * any idx_t derivable from a 64‑bit value.  Uses binary search with
 * 128‑bit multiplication to prevent overflow. */
static idx_t isqrt_idx(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        /* use 128‑bit to avoid overflow of mid*mid */
        __int128 sq = (__int128)mid * mid;
        if (sq <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Multiplicative binomial coefficient.  Computes C(n,k) for 0 ≤ k ≤ n.
 * If k < 0 or k > n, the result is zero.  The computation is done
 * iteratively to keep intermediate values small. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k < 0) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Computes a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Determine the row and column in the triangle
     *  row = floor((sqrt(8*n + 1) - 1)/2)
     *  col = n - row*(row + 1)/2
     * The value 8*n + 1 comfortably fits in 128 bits for all practical n. */
    __int128 t128 = (__int128)8 * n + 1;
    idx_t t = (idx_t)t128;                      /* 8*n+1 as 64‑bit */
    idx_t s = isqrt_idx(t);
    idx_t row = (s - 1) >> 1;                   /* (s-1)/2 */
    idx_t tri = (idx_t)(((__int128)row * (row + 1)) >> 1);
    idx_t col = n - tri;

    /* Compute the triangle entry T(row,col) */
    seq_t sum = 0;
    for (idx_t i = 0; i <= row + 1; ++i) {
        idx_t top = row - i + 1;          /* n-i+1 in the definition */
        seq_t c1 = (i == 0) ? 0 : binom(top, i - 1);
        seq_t c2 = binom(top, i);
        idx_t k3 = (long long)col - (long long)i + 1;
        if (k3 < 0) continue;            /* C(top, k3) = 0 for k3 < 0 */
        seq_t c3 = binom(top, k3);
        sum += c1 * c2 * c3;
    }
    return sum;
}
