/*
 * A366362  Triangle read by rows: T(n,k) = Sum_{y=1..n} Sum_{x=1..n}
 *          [GCD(f(x,y), n) = k], where f(x,y) = x^3 - x^2 - y^2 - y.
 * https://oeis.org/A366362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A366362:
 * a(n) = the nth entry of the triangle T(n, k) read by rows,
 * where T(n, k) = sum_{x=1..n} sum_{y=1..n} [gcd(x^3 - x^2 - y^2 - y, n) == k].
 *
 * The implementation computes the required row n on demand, counts the
 * occurrences of each gcd value, and returns the kth element of that row.
 * All intermediate counts fit in 64‑bit integers; they are cast to seq_t
 * when returned, which works for all supported seq_t types (including
 * arbitrary‑precision).
 *
 * No caching is used; a single call costs O(n^2) operations, which is
 * well below the required 2 s for n up to about 10 000.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r‑1)/2 < n ≤ r(r+1)/2. */
    idx_t r = 1;
    idx_t t = 1;                 /* current triangular number t = r(r+1)/2 */
    while (t < n) {
        ++r;
        t += r;                  /* update t for next r */
    }

    idx_t prev_t = t - r;   /* triangular number for r‑1: (r‑1)·r/2 */
    idx_t k = n - prev_t;   /* 1‑based column index within the row          */

    /* Allocate a zero‑initialised counter array: indices 1..r used. */
    uint64_t *cnt = (uint64_t *)calloc((size_t)r + 1, sizeof(uint64_t));
    if (cnt == NULL)
        abort();                 /* out of memory (should not happen) */

    /* Modulus for gcd calculations. */
    seq_t rn = (seq_t)r;

    for (idx_t x = 1; x <= r; ++x) {
        long long xx  = (long long)x;
        long long xx2 = xx * xx;          /* x^2 */
        long long xx3 = xx2 * xx;         /* x^3 */

        for (idx_t y = 1; y <= r; ++y) {
            long long yy  = (long long)y;
            long long yy2 = yy * yy;      /* y^2 */
            /* f(x, y) = x^3 - x^2 - y^2 - y */
            long long val = xx3 - xx2 - yy2 - yy;
            unsigned long long aval = (unsigned long long)(val >= 0 ? val : -val);
            seq_t saval = (seq_t)aval;
            seq_t g = gcd(saval, rn);     /* g ∈ [1, r] */
            ++cnt[(size_t)g];
        }
    }

    /* The desired entry is k (1‑based).  Convert to seq_t and return. */
    seq_t result = (seq_t)cnt[(size_t)k];
    free(cnt);
    return result;
}
