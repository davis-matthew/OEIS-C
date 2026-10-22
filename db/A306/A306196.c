/*
 * A306196  Irregular triangle read by rows where row n lists the primes 2n -
 *          k, with 1 < k < 2n-1, and if k is composite also 2n - p has to be
 *          prime for some prime divisor p of k.
 * https://oeis.org/A306196
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A306196: a(n) is the n-th prime from the irregular prime triangle
 * described in the OEIS entry (rows indexed starting at 2).
 * The function generates each row on demand and caches the flattened
 * sequence for efficient repeated access.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------- prime sieve (smallest prime factor) ----------------- */

static uint64_t *spf = NULL;            /* spf[x] == smallest prime factor of x */
static idx_t spf_limit = 0;              /* current maximum value stored in spf */

/* Ensure that spf contains data up to at least 'limit' (inclusive) */
static void ensure_spf_limit(idx_t limit)
{
    if (limit <= spf_limit)
        return;

    /* allocate a larger table (grow by factor 1.5 for efficiency) */
    idx_t new_limit = limit + 1;
    uint64_t *new_spf = (uint64_t *)malloc(new_limit * sizeof(uint64_t));
    if (!new_spf)
        abort();                     /* out of memory */

    for (idx_t i = 0; i < new_limit; ++i)
        new_spf[i] = 0;

    for (idx_t i = 2; i < new_limit; ++i) {
        if (new_spf[i] == 0) {                    /* prime */
            new_spf[i] = i;
            if ((uint64_t)i * i < new_limit) {
                for (idx_t j = i * i; j < new_limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* ---------- cache for the linearized triangle sequence --------------- */
static seq_t *cache   = NULL;       /* cache[n] = a(n) for n >= 2            */
static size_t cache_cap   = 0;       /* allocated size of cache[]            */
static size_t cache_size  = 1;       /* largest n for which a(n) is stored   */

/* Grow the cache array to hold at least 'needed' indices */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < needed)
        new_cap <<= 1;   /* double until we have enough space */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Comparator for qsort on seq_t values (assumes seq_t provides < & > operators) */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* --------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* values below the offset are irrelevant */

    /* If already computed, return cached value */
    if (n <= cache_size)
        return cache[n];

    /* Generate rows up to n, appending primes to the cache */
    static idx_t current_row = 2;     /* next row to generate */
    size_t next_index = cache_size + 1;   /* first free index in cache */

    for (idx_t r = current_row; r <= n; ++r) {
        /* Make sure we have prime information up to 2*r. */
        ensure_spf_limit((idx_t)(2 * r));

        /* Prepare a temporary buffer for the row */
        idx_t row_capacity = 2 * r - 3;          /* maximum possible entries */
        seq_t *row_buf = (seq_t *)malloc(row_capacity * sizeof(seq_t));
        if (!row_buf)
            abort();                        /* out of memory */

        size_t row_len = 0;

        /* Scan k = 2 .. 2r-2 (exclusive of 1 and 2r-1). */
        for (idx_t k = 2; k <= 2 * r - 2; ++k) {
            idx_t val_int = (idx_t)(2 * r - k);   /* candidate prime 2r-k */

            /* 2r-k must be prime. */
            if (val_int < 2 || val_int > spf_limit || spf[val_int] != val_int)
                continue;

            /* If k is composite, we must find a prime divisor p of k such that
             * 2r - p is also prime.                                    */
            bool ok = true;
            if (k > 1 && spf[k] != k) { /* k is composite */
                ok = false;
                idx_t tmp = k;
                while (tmp > 1) {
                    idx_t p = spf[tmp];
                    idx_t val_p_int = (idx_t)(2 * r - p);
                    if (val_p_int >= 2 && val_p_int <= spf_limit && spf[val_p_int] == val_p_int) {
                        ok = true;
                        break;
                    }
                    while (tmp % p == 0)
                        tmp /= p;
                }
            }
            if (!ok)
                continue;

            /* Accept this prime. Store it as seq_t. */
            row_buf[row_len++] = (seq_t)val_int;
        }

        /* Sort the row if it contains more than one element. */
        if (row_len > 1)
            qsort(row_buf, row_len, sizeof(seq_t), cmp_seq);

        /* Append the row to the cache. */
        cache_grow(next_index + row_len);
        for (size_t i = 0; i < row_len; ++i)
            cache[next_index++] = row_buf[i];

        free(row_buf);
    }

    /* Update global state. */
    cache_size = next_index - 1;
    current_row = n + 1;

    return cache[n];
}
