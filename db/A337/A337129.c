/*
 * A337129  Triangular array read by rows: T(n,0) = 2^n, T(n,k) =
 *          Sum_{i=n-k..n, j=0..i-n+k, i<>n or j<>k} T(i,j) for k > 0.
 * https://oeis.org/A337129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 42, u32 n <= 118, u64 n <= 432,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337129: Triangular array read by rows
 *
 *   T(n,0) = 2^n
 *   T(n,k) = g(k) * 2^(n-k)  (1 ≤ k ≤ n)
 *
 *   g(1) = 3, g(2) = 16, g(k) = 6*g(k-1) - 4*g(k-2) for k ≥ 3
 *
 *   a(n) is the T(n,k) obtained by traversing the triangle row‑wise
 *   (offset 0).  The row index r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *   and the column index k = n - r(r+1)/2.
 *
 * The implementation computes g(k) on demand and stores it in a
 * statically allocated dynamic array.  All arithmetic is performed
 * directly on seq_t, which may be a fixed‑width unsigned type or an
 * arbitrary‑precision integer class.  No floating‑point or external
 * dependencies other than standard headers are used.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Internal cache for the g(k) sequence */
static seq_t *g_arr = NULL;   /* 1‑based indices, g_arr[0] unused */
static idx_t  g_cap = 0;      /* largest k computed */

/* Compute 2^exp as seq_t */
static seq_t pow2(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r <<= 1;
    return r;
}

/* Ensure that g_arr contains values up to k (inclusive) */
static void ensure_g(idx_t k)
{
    if (k <= g_cap)
        return;

    /* Allocate or grow the array */
    size_t new_cap = (size_t)k;
    seq_t *new_arr = (seq_t *)realloc(g_arr, (new_cap + 1) * sizeof(seq_t));
    if (!new_arr) exit(1);  /* memory allocation failure */

    if (g_cap == 0) {
        /* First allocation: initialise from scratch */
        new_arr[0] = 0;          /* dummy, never used */
        if (new_cap >= 1) new_arr[1] = (seq_t)3;
        if (new_cap >= 2) new_arr[2] = (seq_t)16;
    } else {
        /* We already have g_arr[1] (and maybe g_arr[2]) */
        /* If new_cap reaches 2 and we haven't set it yet, do so */
        if (g_cap < 2 && new_cap >= 2)
            new_arr[2] = (seq_t)16;

        /* Compute further terms using the recurrence */
        idx_t start = g_cap + 1;
        if (start < 3) start = 3;          /* recurrence starts at k=3 */
        for (idx_t i = start; i <= new_cap; ++i) {
            seq_t t1 = new_arr[i - 1] * (seq_t)6;
            seq_t t2 = new_arr[i - 2] * (seq_t)4;
            new_arr[i] = t1 - t2;
        }
    }

    g_arr = new_arr;
    g_cap = new_cap;
}

/* Return the n‑th term of sequence A337129 (0‑based indexing) */
seq_t a(idx_t n)
{
    if (n == 0)
        return pow2(0);      /* 1 */

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    idx_t r = ans;

    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;    /* column index */

    if (k == 0) {
        return pow2(r);              /* T(r,0) */
    }

    ensure_g(k);

    seq_t pow_val = pow2(r - k);    /* 2^(r-k) */
    return pow_val * g_arr[k];
}
