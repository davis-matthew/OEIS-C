/*
 * A276189  Triangle read by rows: T(n,k) = 2*(6*k^2 + 1)*(n + 1 - k) for 0 <
 *          k <= n; for k = 0, T(n,0) = n + 1.
 * https://oeis.org/A276189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) outputs the nth term of the triangle read row‑wise. * Row r
 *            has r+1 terms: * T(r,0) = r+1; * T(r,k) = 2*(6*k*k+1)*(r+1-k)
 *            for 1 ≤ k ≤ r. * The index n is zero‑based and the function
 *            works for any n ≥ 0
 * Range:     u8 n <= 7, u16 n <= 580, u32 n <= 11475, u64 n <= 11475,
 *            u128 n <= 11475, big any n
 * Validated: standard, pass, 82 terms through n = 8501 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* Convert the index into the triangle row/column */
    seq_t tn = (seq_t)n;                 /* n as seq_t for calculations      */
    seq_t disc = (seq_t)1 + (seq_t)8 * tn;   /* 1 + 8*n */
    seq_t s     = floor_sqrt(disc);          /* floor(sqrt(1+8*n))           */
    idx_t r = (idx_t)((s - (seq_t)1) / (seq_t)2); /* approximate row                */

    /* Adjust r to satisfy r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    seq_t rseq = (seq_t)r;
    while (( (rseq + (seq_t)1) * (rseq + (seq_t)2) ) / (seq_t)2 <= tn)
    {
        r++;
        rseq = (seq_t)r;
    }
    while ((rseq * (rseq + (seq_t)1)) / (seq_t)2 > tn)
    {
        r--;
        rseq = (seq_t)r;
    }

    /* Position within the row */
    seq_t terms_before = (rseq * (rseq + (seq_t)1)) / (seq_t)2;
    idx_t k = (idx_t)(tn - terms_before);   /* 0 ≤ k ≤ r */

    if (k == 0)
        return rseq + (seq_t)1;             /* T(r,0) = r+1 */

    /* T(r,k) = 2 * (6*k*k + 1) * (r+1 - k) */
    seq_t kk = (seq_t)k * (seq_t)k;
    seq_t factor = (seq_t)6 * kk + (seq_t)1;
    seq_t result = (seq_t)2 * factor * (rseq + (seq_t)1 - (seq_t)k);
    return result;
}
