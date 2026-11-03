/*
 * A325655  Triangle read by rows: T(n, k) = (1/4)*(2*(-1 + (-1)^n)*k -
 *          2*k^2*n + n*(2 - (-1)^k - (-1)^n + 2*n^2)), with 0 <= k < n.
 * https://oeis.org/A325655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth entry of the triangle read by rows T(n,k) =
 *            (1/4)*(2*(-1 + (-1)^n)*k - 2*k^2*n + n*(2 - (-1)^k - (-1)^n +
 *            2*n^2)) with 1 ≤ n and 0 ≤ k < n. The sequence is obtained by
 *            flattening this triangle row by row: a(1)=T(1,0), a(2)=T(2,0),
 *            a(3)=T(2,1), a(4)=T(3,0), …
 * Range:     u8 n <= 7, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return the k‑subdiagonal sum for the given row n and column k. */
static seq_t triangle_value(idx_t n, idx_t k)
{
    /* Parity of n and k */
    bool n_even = !(n & 1);
    bool k_even = !(k & 1);

    seq_t N = (seq_t)n;
    seq_t K = (seq_t)k;
    seq_t N2 = N * N;  /* n^2 (fits in seq_t for n ≤ 10^5) */
    seq_t K2 = K * K;  /* k^2 */

    seq_t val;
    if (n_even && k_even) {
        /* (1/2) * n * (n^2 - k^2) */
        val = N * (N2 - K2);
    } else if (n_even && !k_even) {
        /* (1/2) * n * (n^2 - k^2 + 1) */
        val = N * (N2 - K2 + 1);
    } else if (!n_even && k_even) {
        /* (1/2) * ( n*(n^2 - k^2 + 1) - 2*k ) */
        val = N * (N2 - K2 + 1) - 2 * K;
    } else { /* both odd */
        /* (1/2) * ( n*(n^2 - k^2 + 2) - 2*k ) */
        val = N * (N2 - K2 + 2) - 2 * K;
    }
    return val / 2;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n >
       (r‑1)*r/2 < n.  n ≥ 1. */
    seq_t t = (seq_t)8 * (seq_t)n + 1;   /* 8*n + 1 */
    seq_t s = floor_sqrt(t);            /* floor(sqrt(8*n+1)) */
    idx_t r = (idx_t)((s - 1) / 2);      /* initial guess */

    /* Adjust r upward if necessary (due to floor sqrt) */
    seq_t curr = (seq_t)r * ((seq_t)r + 1) / 2;
    while (curr < (seq_t)n) {
        r++;
        curr = (seq_t)r * ((seq_t)r + 1) / 2;
    }

    /* Previous total terminates at row r‑1 */
    seq_t prev = ((seq_t)(r - 1)) * (seq_t)r / 2;

    /* Index within the row (0‑based) */
    idx_t k = (idx_t)(((seq_t)n - prev) - 1);

    return triangle_value(r, k);
}
