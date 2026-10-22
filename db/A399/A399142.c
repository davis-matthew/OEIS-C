/*
 * A399142  Triangle read by rows: T(n,k) counts trees on [n] rooted at 1
 *          together with a level assignment L:[n]->{0,...,k} that uses every
 *          level, has L(1)=0, and strictly increases from parent to child,
 *          0<=k<=n-1.
 * https://oeis.org/A399142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n‑th entry of the triangle T(n,k) read row by row,
 *            where T(n,k) counts trees on [n] rooted at 1 together with a
 *            level assignment L:[n]→{0,…,k} that uses every level, has
 *            L(1)=0, and strictly increases from parent to child
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions from the utils repository */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

/* Compatibility cast: works in both C and C++ */
#ifdef __cplusplus
#define CAST(ptr, to) reinterpret_cast<to>(ptr)
#else
#define CAST(ptr, to) ((to)(ptr))
#endif

/* Cached triangle */
static seq_t **T = NULL;          /* rows indexed from 1 */
static idx_t T_rows = 0;          /* highest row already computed */

/* Find row r such that r*(r+1)/2 >= n  (row numbering starts at 1) */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;          /* row never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s >= (unsigned __int128)n) high = mid;
        else                           low = mid + 1;
    }
    return low;
}

/* Extend the triangle to contain rows up to max_r */
static void ensure_T_rows(idx_t max_r)
{
    if (max_r <= T_rows) return;          /* nothing to do */

    /* Grow the row pointer array */
    T = CAST(realloc(T, sizeof(seq_t *) * (max_r + 1)), seq_t **);
    if (!T) abort();                      /* allocation failure */

    /* Allocate new rows and set base cases */
    for (idx_t n = T_rows + 1; n <= max_r; ++n) {
        T[n] = CAST(calloc((size_t)n, sizeof(seq_t)), seq_t *);
        if (!T[n]) abort();
        if (n == 1) T[n][0] = (seq_t)1;   /* T(1,0) = 1 */
    }

    /* Compute triangular entries using the recurrence */
    for (idx_t n = T_rows + 1; n <= max_r; ++n) {
        for (idx_t k = 1; k <= n - 1; ++k) {
            seq_t sum = 0;
            for (idx_t j = k; j <= n - 1; ++j) {
                seq_t term = big_pow((seq_t)j, (seq_t)(n - j));
                term *= binomial(n - 1, j - 1);
                term *= T[j][k - 1];
                sum += term;
            }
            T[n][k] = sum;
        }
    }

    T_rows = max_r;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;         /* T(1,0) */

    /* Locate the row and column corresponding to the flattened index */
    idx_t r = find_row(n);

    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;            /* previous count */

    idx_t k = n - prev - 1;                 /* 0‑based column */

    /* Ensure the required rows are computed */
    if (r > T_rows) ensure_T_rows(r);

    return T[r][k];
}
