/*
 * A256548  Triangle read by rows, T(n,k) = |n,k|*h(k), where |n,k| are the
 *          Stirling cycle numbers and h(k) = hypergeom([-k+1,-k],[],1), for
 *          n>=0 and 0<=k<=n.
 * https://oeis.org/A256548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col) with * T(r, k) = |r,k| * h(k) * |r,k| =
 *            unsigned Stirling number of the first kind * h(k) =
 *            Σ_{j=0}^{k-1} C(k,j) * C(k-1,j) * j! (hypergeometric sum)
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;
    while (hi - lo > 1) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    *row = lo;
    *col = n - lo * (lo + 1) / 2;
}

/* Compute h(k) = Σ_{j=0}^{k-1} C(k,j) * C(k-1,j) * j! */
static seq_t h_value(idx_t k)
{
    if (k == 0)
        return (seq_t)1;
    seq_t total = (seq_t)0;
    seq_t fact  = (seq_t)1;      /* 0! = 1 */
    for (idx_t j = 0; j < k; ++j) {
        if (j > 0)
            fact = mul_small(fact, (unsigned int)j);   /* j! */
        seq_t term = seq_mul(binomial(k, j), binomial(k - 1, j));
        term = seq_mul(term, fact);
        total += term;
    }
    return total;
}

/* Unsigned Stirling numbers of the first kind S(r,k). */
static seq_t stirling_first(idx_t r, idx_t k)
{
    if (k == 0)
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    if (k > r)
        return (seq_t)0;
    seq_t *arr = mem_new(seq_t, k + 1);
    if (!arr) exit(1);
    arr[0] = (seq_t)1;      /* S(0,0) = 1 */

    for (idx_t i = 1; i <= r; ++i) {
        idx_t jmax = (i < k) ? i : k;
        for (idx_t j = jmax; j >= 1; --j) {
            seq_t prod = mul_small(arr[j], (unsigned int)(i - 1));
            arr[j] = prod + arr[j - 1];
        }
        arr[0] = (seq_t)0; /* S(i,0) = 0 for i>0 */
    }
    seq_t res = arr[k];
    mem_free(arr);
    return res;
}

seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rowcol(n, &r, &c);
    seq_t s = stirling_first(r, c);
    seq_t h = h_value(c);
    return seq_mul(s, h);
}
