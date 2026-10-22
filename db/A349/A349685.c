/*
 * A349685  Irregular triangle read by rows: the n-th row contains the
 *          elements in the continued fraction of the abundancy index of n.
 * https://oeis.org/A349685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1463, u32 n <= 9490, u64 n <= 9490,
 *            u128 n <= 9490, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of the irregular triangle read by rows
 * of OEIS A349685. Each row n contains the continued‑fraction
 * coefficients of the abundancy index σ(n)/n.
 *
 * The function precomputes a cumulative length table for rows as
 * needed, then identifies the row that contains the requested index
 * and returns the appropriate CF coefficient.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static idx_t *cum = NULL;          /* cumulative lengths: cum[0] = 0 */
static size_t cum_cnt = 0;          /* number of entries stored (rows + 1) */
static size_t cum_cap = 0;          /* allocated capacity of cum */

/* Ensure that cum has room for at least 'need' entries (indices 0 … need-1). */
static void ensure_cum_size(size_t need)
{
    if (need <= cum_cap) return;
    size_t newcap = cum_cap ? cum_cap << 1 : 64;
    while (newcap < need) newcap <<= 1;
    idx_t *newarr = (idx_t *)malloc(newcap * sizeof(idx_t));
    if (!newarr) exit(1);  /* allocation failed */
    for (size_t i = 0; i < cum_cnt; ++i) newarr[i] = cum[i];
    free(cum);
    cum = newarr;
    cum_cap = newcap;
}

/* Compute σ(n) for a 64‑bit integer n using prime factorisation. */
static seq_t sigma_uint64(uint64_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* Compute (p^(e+1) - 1) / (p - 1) */
        seq_t pe = 1;
        for (uint64_t j = 0; j < e + 1; ++j) pe *= p;   /* p^(e+1) */
        seq_t numerator   = pe - 1;
        seq_t denominator = (seq_t)p - 1;
        seq_t factor = numerator / denominator;
        result *= factor;
    }
    return result;
}

/* Length of the continued fraction of σ(m)/m. */
static int cf_length_uint64(uint64_t m)
{
    seq_t num = sigma_uint64(m);
    seq_t den = (seq_t)m;
    seq_t g   = gcd(num, den);
    num /= g;
    den /= g;

    int len = 0;
    while (den != 0) {
        seq_t q = num / den;
        (void)q; /* unused here, but kept for symmetry */
        seq_t r = num % den;
        num = den;
        den = r;
        ++len;
    }
    return len;
}

/* k‑th coefficient (1‑based) of the continued fraction of σ(m)/m. */
static seq_t cf_kth_term_uint64(uint64_t m, int k)
{
    seq_t num = sigma_uint64(m);
    seq_t den = (seq_t)m;
    seq_t g   = gcd(num, den);
    num /= g;
    den /= g;

    int pos = 1;
    while (den != 0) {
        seq_t q = num / den;
        if (pos == k) return q;
        seq_t r = num % den;
        num = den;
        den = r;
        ++pos;
    }
    return 0;            /* should never reach */
}

/* Binary search: find the smallest r with cum[r] >= n.  r is 1‑based. */
static size_t find_row(const idx_t *cum_arr, size_t cnt, idx_t n)
{
    size_t l = 1;          /* cum[0] == 0, never matches n>=1 */
    size_t r = cnt - 1;    /* last valid index is cnt-1 = max_row */

    while (l < r) {
        size_t m = (l + r) >> 1;
        if (cum_arr[m] >= n) r = m;
        else                 l = m + 1;
    }
    return l;
}

/* Main sequence function.  a(1) is the first term of the triangle. */
seq_t a(idx_t n)      /* 1‑based index */
{
    if (n == 0) return 0;   /* below offset – undefined, but safe */

    /* Lazy initialisation of cumulative table. */
    if (cum == NULL) {
        ensure_cum_size(2);          /* indices 0 and 1 */
        cum[0] = 0;
        cum_cnt = 1;                 /* only cum[0] stored so far */
    }

    /* Extend the table until it covers the requested index n. */
    while (cum_cnt <= (size_t)n) {          /* cum_cnt == max_row+1 */
        uint64_t row = (uint64_t)(cum_cnt); /* next row number */
        int len = cf_length_uint64(row);

        idx_t newcum = cum[cum_cnt - 1] + (idx_t)len;
        ensure_cum_size(cum_cnt + 1);
        cum[cum_cnt] = newcum;
        ++cum_cnt;
    }

    /* Locate the row that contains position n. */
    size_t row_idx = find_row(cum, cum_cnt, n);   /* 1‑based row index */
    idx_t prev_cum = cum[row_idx - 1];
    int k = (int)(n - prev_cum);                 /* 1‑based within the row */

    return cf_kth_term_uint64((uint64_t)row_idx, k);
}
