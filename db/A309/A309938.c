/*
 * A309938  Triangle read by rows: T(n,k) is the number of compositions of n
 *          with k parts and differences all equal to 1 or -1.
 * https://oeis.org/A309938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) term of the triangle T(r,k) read by rows,
 *            where T(r,k) counts the compositions of r into k parts with
 *            adjacent parts differing by ±1. The function maps index n to row
 *            r and column k by the triangular index: r = min{R : R(R+1)/2 ≥
 *            n}, k = n – (r−1)r/2, and returns T(r,k)
 * Range:     u8 n <= 91, u16 n <= 1736, u32 n <= 6377, u64 n <= 9824,
 *            u128 n <= 9824, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Maximum row computed so far (dimension‑1 for the square arrays). */
static size_t current_max_r = 0;

/* Number of columns in the square storage.  stride = current_max_r + 1. */
static size_t stride = 0;

/* Triangle values T[r][k], 1 ≤ k ≤ r ≤ current_max_r. */
static seq_t *T_arr = NULL;

/* DP arrays for current and next length.  Each is a square of size stride. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Compute the triangular number in a type‑safe way. */
static inline seq_t triangular(idx_t r)
{
    return (seq_t)r * ((seq_t)r + 1) / 2;
}

/* Rebuild the triangle up to new_max_r (inclusive).  All arrays are
   re‑allocated with size (new_max_r+1)².  The function is O(new_max_r³),
   but new_max_r is only about √(2n) for the requested n. */
static void recompute_up_to(size_t new_max_r)
{
    size_t new_stride = new_max_r + 1;
    size_t total = new_stride * new_stride;

    free(T_arr);
    free(dp_cur);
    free(dp_next);

    T_arr   = (seq_t *)calloc(total, sizeof(seq_t));
    dp_cur  = (seq_t *)calloc(total, sizeof(seq_t));
    dp_next = (seq_t *)calloc(total, sizeof(seq_t));

    if (!T_arr || !dp_cur || !dp_next) return; /* out of memory – caller handles */

    /* length 1: one part compositions. */
    for (idx_t val = 1; val <= new_max_r; ++val) {
        size_t idx = (size_t)val * new_stride + val;   /* sum=val, last=val */
        dp_cur[idx] = 1;
        T_arr[(size_t)val * new_stride + 1] = 1;        /* T[val][1] = 1 */
    }

    /* lengths 2 .. new_max_r */
    for (size_t len = 2; len <= new_max_r; ++len) {
        /* clear next dp array */
        for (size_t i = 0; i < total; ++i) dp_next[i] = 0;

        /* transitions */
        for (idx_t s = 1; s <= new_max_r; ++s) {
            for (idx_t last = 1; last <= new_max_r; ++last) {
                seq_t cur = dp_cur[(size_t)s * new_stride + last];
                if (!cur) continue;

                /* increase by 1 */
                idx_t nl = last + 1;
                idx_t ns = s + nl;
                if (nl <= new_max_r && ns <= new_max_r)
                    dp_next[(size_t)ns * new_stride + nl] += cur;

                /* decrease by 1 */
                if (last > 1) {
                    nl = last - 1;
                    ns = s + nl;
                    if (ns <= new_max_r)
                        dp_next[(size_t)ns * new_stride + nl] += cur;
                }
            }
        }

        /* compute T[sum][len] from dp_next */
        for (idx_t s = 1; s <= new_max_r; ++s) {
            seq_t sum = 0;
            for (idx_t last = 1; last <= new_max_r; ++last)
                sum += dp_next[(size_t)s * new_stride + last];
            T_arr[(size_t)s * new_stride + len] = sum;
        }

        /* swap dp arrays for the next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    stride = new_stride;
    current_max_r = new_max_r;
}

/* Public function to return the a(n)-term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety; sequence is 1‑indexed */

    /* Find row r such that r(r+1)/2 ≥ n.  Use double sqrt for an initial guess. */
    double approx = sqrt((double)(8.0 * (double)n + 1.0));
    idx_t r = (idx_t)((approx - 1.0) * 0.5);

    /* Correct any rounding errors with integer comparisons. */
    while (triangular(r) < n) ++r;
    while (r > 0 && triangular(r - 1) >= n) --r;

    /* Column k within the row. */
    idx_t k = n - (idx_t)triangular(r - 1);

    /* Ensure we have pre‑computed triangle up to row r. */
    if ((size_t)r > current_max_r) recompute_up_to((size_t)r);

    /* Return the cached value. */
    return T_arr[(size_t)r * stride + (size_t)k];
}
