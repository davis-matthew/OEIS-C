/*
 * A360049  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(n+2,3*k+2) *
 *          Catalan(k).
 * https://oeis.org/A360049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(n+2,3*k+2) *
 *            Catalan(k)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 27, i64 n <= 58, i128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* cache for already‑computed a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;      /* first index NOT computed yet */

/* cache for Catalan numbers */
static seq_t *catalan = NULL;
static size_t cat_cap = 0;
static size_t cat_len = 0;        /* number of Catalan terms stored */

/* ensure cache has room up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed)
    {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* ensure catalan array has entries 0..kmax (inclusive) */
static void ensure_catalan(size_t kmax)
{
    size_t needed = kmax + 1;
    if (cat_cap < needed)
    {
        size_t new_cap = (cat_cap == 0) ? 1 : cat_cap * 2;
        if (new_cap < needed) new_cap = needed;
        catalan = (seq_t*)realloc(catalan, new_cap * sizeof(seq_t));
        cat_cap = new_cap;
    }
    if (cat_len < needed)
    {
        if (cat_len == 0)
        {
            catalan[0] = 1;
            cat_len = 1;
        }
        for (size_t i = cat_len; i < needed; ++i)
        {
            /* Catalan(i) = Catalan(i-1) * 2*(2*(i-1)+1) / (i+1) */
            seq_t prev = catalan[i - 1];
            seq_t num   = prev * (seq_t)(2 * (2 * (i - 1) + 1));
            seq_t den   = (seq_t)(i + 1);
            catalan[i] = num / den;
        }
        cat_len = needed;
    }
}

seq_t a(idx_t n)
{
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    ensure_cache(n);

    /* compute new values from cache_len up to and including n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        idx_t limit = (idx_t)(i / 3);          /* floor(i/3) */
        seq_t sum = 0;

        /* binomial(i+2, 2) as initial value (r = 2) */
        seq_t binom = ((seq_t)(i + 2) * (seq_t)(i + 1)) / 2;
        idx_t r = 2;                          

        /* ensure Catalan numbers up to 'limit' exist */
        ensure_catalan((size_t)limit);

        for (idx_t k = 0; k <= limit; ++k)
        {
            seq_t term = binom * catalan[(size_t)k];
            if (k & 1)
                term = -term;
            sum += term;

            if (k == limit) break;

            /* update binom to C(i+2, r+3) */
            for (idx_t step = 0; step < 3; ++step)
            {
                seq_t num = (seq_t)(i + 2 - r - step);
                seq_t den = (seq_t)(r + 1 + step);
                binom = (binom * num) / den;
            }
            r += 3;
        }

        cache[i] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
