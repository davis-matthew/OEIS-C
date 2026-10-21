/*
 * A276995  Triangle read by rows, T(n,k) =
 *          k^(n-k)*(n-k)!*Sum_{j=0..n-k}(-1)^j/j! for 0<=k<=n.
 * https://oeis.org/A276995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle read by rows. Let r be
 *            the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. Let k = n
 *            – r*(r+1)/2 (the position in row r) and m = r – k. Then a(n) =
 *            T(r,k) = k^m · D(m) where D(m) is the derangement number
 *            (D(0)=1, D(1)=0, D(m) = (m-1)*(D(m-1)+D(m-2)) for...
 * Range:     u8 n <= 28, u16 n <= 46, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdlib.h>
/* Utility functions – these are provided by the build system */
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */

/* Cache for derangement numbers D(m) */
static seq_t *D_cache   = NULL;  /* pointer to array of D values      */
static size_t D_cap     = 0;     /* allocated capacity of D_cache    */
static size_t D_size    = 0;     /* number of valid entries (last= D_size-1) */

/* Ensure that D_cache contains values up to index n (inclusive) */
static void ensure_D(size_t n)
{
    /* Allocate or grow the cache if necessary */
    if (D_cap <= n) {
        size_t new_cap = D_cap == 0 ? 1 : D_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(D_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        D_cache = tmp;
        D_cap   = new_cap;
    }

    /* Compute missing derangements up to n */
    if (D_size <= n) {
        for (size_t i = D_size; i <= n; ++i) {
            if (i == 0) {
                D_cache[i] = (seq_t)1;
            } else if (i == 1) {
                D_cache[i] = (seq_t)0;
            } else {
                /* D(i) = (i-1) * (D(i-1) + D(i-2)) */
                seq_t sum = D_cache[i-1] + D_cache[i-2];
                D_cache[i] = seq_mul(sum, (seq_t)(i-1));
            }
        }
        D_size = n + 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n ) {
        ++r;
    }

    /* Position within the row (0‑based) */
    idx_t k = n - (r * (r + 1)) / 2;
    /* m = r – k = n – k (the exponent / derangement index) */
    idx_t m = r - k;

    /* Ensure we have D(m) ready */
    ensure_D((size_t)m);

    /* Compute k^m as a big integer (returns seq_t) */
    seq_t pow_val = big_pow((seq_t)k, (seq_t)m);

    /* Result is k^m * D(m) */
    return seq_mul(pow_val, D_cache[(size_t)m]);
}
