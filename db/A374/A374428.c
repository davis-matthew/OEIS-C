/*
 * A374428  Triangle read by rows: T(n, k) = n! * 2^k * hypergeom([-k], [-n],
 *          1/2).
 * https://oeis.org/A374428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A374428: Triangle read by rows
 *   a(n) = T(r,k) where r = floor( (sqrt(8n+1)-1)/2 )
 *          and k = n - r(r+1)/2
 *   T(r,k) = Σ_{j=0..k} 2^(k−j) * C(k,j) * (r−j)!
 *   (C(k,j) = binomial coefficient)
 *
 * This implementation uses cached factorials and powers of two.
 * Caching keeps the routine O(n) for n up to ~10⁴ and works with
 * arbitrary‑precision seq_t types.                  */

#include <stdlib.h>
#include <stdint.h>

/* Helpers for caching factorials */
static seq_t *fact = NULL;      /* fact[i] = i!   */
static idx_t fact_cap = 0;
static void ensure_fact(idx_t up_to)
{
    if (up_to < fact_cap) return;
    size_t new_cap = (size_t)(up_to + 1);
    seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    fact = tmp;
    if (fact_cap == 0) fact[0] = (seq_t)1;
    for (idx_t i = fact_cap; i <= up_to; ++i)
    {
        if (i == 0) fact[i] = (seq_t)1;
        else        fact[i] = fact[i-1] * (seq_t)(i);
    }
    fact_cap = new_cap;
}

/* Helpers for caching powers of two */
static seq_t *p2 = NULL;        /* p2[i] = 2^i   */
static idx_t p2_cap = 0;
static void ensure_pow2(idx_t up_to)
{
    if (up_to < p2_cap) return;
    size_t new_cap = (size_t)(up_to + 1);
    seq_t *tmp = (seq_t *)realloc(p2, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    p2 = tmp;
    if (p2_cap == 0) p2[0] = (seq_t)1;
    for (idx_t i = p2_cap; i <= up_to; ++i)
    {
        if (i == 0) p2[i] = (seq_t)1;
        else        p2[i] = p2[i-1] * (seq_t)2;
    }
    p2_cap = new_cap;
}

/* Find row r (0‑based) such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
static idx_t find_row(idx_t n)
{
    if (n == 0) return 0;
    idx_t lo = 0, hi = 1;
    /* Grow hi until it exceeds n */
    while (((__uint128_t)hi * (hi + 1)) / 2 <= n) {
        hi <<= 1;
        if (hi == 0) {          /* overflow guard, though not reached for n≤10⁴ */
            hi = ~((idx_t)0);
            break;
        }
    }
    /* Binary search for largest r with r(r+1)/2 ≤ n */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (((__uint128_t)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0)=1 */

    idx_t row = find_row(n);
    idx_t cumulative = (idx_t)((__uint128_t)row * (row + 1) / 2);
    idx_t col  = n - cumulative;      /* k = col */

    if (col > row) return (seq_t)0;    /* safety, should never happen */

    ensure_fact(row);
    ensure_pow2(row);                  /* max needed k ≤ row */

    seq_t result = (seq_t)0;
    seq_t bin    = (seq_t)1;           /* binomial(k,0) */

    for (idx_t j = 0; j <= col; ++j)
    {
        seq_t pow2 = p2[col - j];
        seq_t term = pow2 * bin * fact[row - j];
        result += term;

        if (j < col)                   /* update binomial(k, j+1) */
            bin = bin * (seq_t)(col - j) / (seq_t)(j + 1);
    }
    return result;
}
