/*
 * A292611  Skip 3 triangle numbers, take 1 triangle number, skip 4 triangle
 *          numbers, take 2 triangle numbers, skip 5 triangle numbers, take 3
 *          triangle numbers, ...
 * https://oeis.org/A292611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A292611 – Skip 3 triangle numbers, take 1, skip 4, take 2, skip 5, take 3, …
 *
 * The sequence is the list of triangular numbers T(m)=m*(m+1)/2 whose indices 'm'
 * belong to the “take” blocks of the pattern Skips/Take: for block k (k≥1),
 *   skip k+2 numbers, then take k numbers.
 *
 * For a given n≥1 we need to find the k‑th block that contains the nth term,
 * find the offset within that block, and compute the corresponding m.
 *
 * If k is the minimal integer with k(k+1)/2 ≥ n, then
 *   m = k*(k+2) + n – (k-1)k/2
 * and a(n) = T(m) = m*(m+1)/2
 *
 * The function below implements this formula in constant time.
 */

#include <stdint.h>

/* Helper: minimal k with k(k+1)/2 >= n (binary search, no overflow) */
static idx_t minimal_k(idx_t n)
{
    idx_t lo = 1, hi = n;          /* upper bound is n itself */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence entry function */
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for n<1, return 0 as safe fallback */
        return 0;

    idx_t k = minimal_k(n);                /* block number contains term n   */
    idx_t sum_before = (k - 1) * k / 2;     /* terms before block k          */

    seq_t m = (seq_t)k * (k + 2)            /* k(k+2)                         */
              + (seq_t)n
              - (seq_t)sum_before;          /* plus offset within block       */

    return m * (m + 1) / 2;                 /* triangular number T(m)         */
}
