/*
 * A373068  Number of partitions of n such that (smallest part) >= 3*(number
 *          of parts).
 * https://oeis.org/A373068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373068: Number of partitions of n such that (smallest part) >= 3*(number of parts).
   Formula: a(n) = Σ_{k≥0} p_k(n – 3k²)
   where p_k(m) is the number of partitions of m into parts ≤ k.
   DP: p_k(m) = p_{k-1}(m) + p_k(m−k)   (m ≥ k), p_k(0)=1, p_0(m)=δ_{m,0}. */

#include <stdlib.h>
#include <string.h>

/* Integer square‑root (binary search). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq <= n)  low = mid + 1;
        else          high = mid - 1;
    }
    return high;
}

/* --------------------------------------------------------------------- */
/*  Dynamic DP table for p_k(m).  The table is rectangular:
        rows  : k = 0 .. Kmax
        cols  : m = 0 .. Nmax
   Stored as a flat array P[k*(Nmax+1)+m].  */
static idx_t Nmax = 0;          /* largest m currently stored */
static idx_t Kmax = 0;          /* largest k currently stored */
static seq_t *P    = NULL;      /* table data */

/* Expand the table so that it covers all values up to n. */
static void ensure_capacity(idx_t n)
{
    idx_t new_nmax = n;                                       /* rows for m */
    idx_t new_kmax = (n >= 3) ? isqrt_u64(n / 3) : 0;          /* rows for k */

    if (P != NULL && new_nmax <= Nmax && new_kmax <= Kmax)    /* already enough */
        return;

    /* Allocate new table. */
    size_t stride_new = (size_t)(new_nmax + 1);
    size_t new_total  = (size_t)(new_kmax + 1) * stride_new;
    seq_t *newP = (seq_t *)calloc(new_total, sizeof(seq_t));
    if (!newP) exit(1);   /* out‑of‑memory */

    /* Set base case: p_0(0) = 1.  (calloc already gave 0 everywhere.) */
    newP[0] = 1;

    /* Copy existing data that still fits in the new array. */
    if (P) {
        size_t stride_old = (size_t)(Nmax + 1);
        for (idx_t k = 0; k <= Kmax; ++k) {
            memcpy(newP + k * stride_new,
                   P + k * stride_old,
                   stride_old * sizeof(seq_t));
        }
        free(P);
    }

    /* Update rows that already existed (k ≤ min(old Kmax, new Kmax)). */
    for (idx_t k = 0; k <= (new_kmax < Kmax ? new_kmax : Kmax); ++k) {
        if (k == 0) continue;   /* p_0 row already correct. */
        for (idx_t m = Nmax + 1; m <= new_nmax; ++m) {
            if (m < k) {
                newP[k * stride_new + m] = newP[(k - 1) * stride_new + m];
            } else {
                newP[k * stride_new + m] =
                    newP[(k - 1) * stride_new + m] +
                    newP[k * stride_new + (m - k)];
            }
        }
    }

    /* Compute entirely new rows (k > old Kmax). */
    for (idx_t k = Kmax + 1; k <= new_kmax; ++k) {
        for (idx_t m = 0; m <= new_nmax; ++m) {
            if (m == 0) {
                newP[k * stride_new + m] = 1;            /* p_k(0)=1 */
            } else {
                if (m < k) {
                    newP[k * stride_new + m] = newP[(k - 1) * stride_new + m];
                } else {
                    newP[k * stride_new + m] =
                        newP[(k - 1) * stride_new + m] +
                        newP[k * stride_new + (m - k)];
                }
            }
        }
    }

    P     = newP;
    Nmax  = new_nmax;
    Kmax  = new_kmax;
}

/* --------------------------------------------------------------------- */
/*  Required sequence function. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    seq_t total = 0;
    idx_t stride = Nmax + 1;
    idx_t k_limit = (n >= 3) ? isqrt_u64(n / 3) : 0;

    for (idx_t k = 0; k <= k_limit; ++k) {
        idx_t m = n - 3 * k * k;
        total += P[k * stride + m];
    }
    return total;
}
