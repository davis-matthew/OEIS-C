/*
 * A360935  Expansion of e.g.f. Sum_{k>=0} exp((k^k - 1)*x) * x^k/k!.
 * https://oeis.org/A360935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} (k^k – 1)^{n‑k} * binomial(n,k) (OEIS A360935)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc, abort */

/* Utility: big_pow(base,exp) is provided in the utils directory. */
#include "../utils/big_pow.h"

/* ------------------------------------------------------------------ */
/*  Cache and helper data                                              */
/* ------------------------------------------------------------------ */
static seq_t *row      = NULL;   /* current row of S values (size m+1)          */
static size_t row_cap  = 0;      /* allocated capacity of row                 */

static seq_t *u_vals   = NULL;   /* u_vals[k] = k^k – 1 (size up to max k)     */
static size_t u_cap    = 0;      /* allocated capacity of u_vals              */
static size_t u_len    = 0;      /* number of u_vals already computed         */

static seq_t *cache    = NULL;   /* cached a(n) values (size max_n+1)         */
static size_t cache_cap= 0;      /* allocated capacity of cache              */
static size_t cache_len= 0;      /* number of cached a(n) values             */

/* ------------------------------------------------------------------ */
/*  Ensure that an array can hold at least `needed` elements          */
/* ------------------------------------------------------------------ */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap) : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    *arr   = tmp;
    *cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Compute (k^k – 1) for all k up to `max_k` (inclusive)            */
/* ------------------------------------------------------------------ */
static void ensure_u_values(idx_t max_k)
{
    if ((size_t)max_k + 1 <= u_len)
        return;
    ensure_capacity(&u_vals, &u_cap, (size_t)max_k + 1);

    for (size_t i = u_len; i <= (size_t)max_k; ++i) {
        if (i == 0) {
            /* 0^0 is defined as 1; minus 1 gives 0. */
            u_vals[i] = (seq_t)0;
        } else {
            seq_t pow = big_pow((seq_t)i, (seq_t)i);
            pow -= (seq_t)1;
            u_vals[i] = pow;
        }
    }
    u_len = (size_t)max_k + 1;
}

/* ------------------------------------------------------------------ */
/*  Ensure that the cache holds a(n) for all n <= new_len-1          */
/* ------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t new_len)
{
    if (new_len <= cache_cap)
        return;
    ensure_capacity(&cache, &cache_cap, new_len);
}

/* ------------------------------------------------------------------ */
/*  Grow cache up to at least index `n`                               */
/* ------------------------------------------------------------------ */
static void grow_cache_to(idx_t n)
{
    if ((size_t)n < cache_len)
        return;

    /* Initialization on first call */
    if (cache_len == 0) {
        ensure_capacity(&row, &row_cap, 1);
        row[0] = (seq_t)1;          /* S(0,0) = 1 */
        ensure_u_values(0);         /* u[0] = 0 */
        ensure_cache_capacity(1);
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        cache_len = 1;
    }

    for (idx_t m = (idx_t)cache_len; m <= n; ++m) {
        ensure_u_values(m - 1);      /* need u[0..m-1] */

        /* Guarantee row can store indices up to m. */
        ensure_capacity(&row, &row_cap, (size_t)m + 1);
        seq_t n_seq = (seq_t)m;

        /* Update existing entries (k = m-1 .. 1) in decreasing order. */
        for (idx_t k = m - 1; k > 0; --k) {
            seq_t tmp = row[k];
            tmp *= u_vals[k];
            tmp *= n_seq;
            tmp /= (seq_t)(m - k);    /* exact division */
            row[k] = tmp;
        }

        /* k = 0 case */
        seq_t tmp0 = row[0];
        tmp0 *= u_vals[0];
        tmp0 *= n_seq;
        tmp0 /= (seq_t)m;            /* m - 0 == m */
        row[0] = tmp0;

        /* Set the diagonal element S(m,m) = 1. */
        row[m] = (seq_t)1;

        /* Sum row to obtain a(m). */
        seq_t sum = 0;
        for (idx_t i = 0; i <= m; ++i)
            sum += row[i];

        /* Store a(m) in the cache. */
        ensure_cache_capacity((size_t)m + 1);
        cache[m] = sum;
        cache_len = (size_t)m + 1;
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    grow_cache_to(n);
    return cache[(size_t)n];
}
