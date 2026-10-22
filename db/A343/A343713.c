/*
 * A343713  Table read by rows: row n lists the residues of the triangular
 *          numbers modulo n.
 * https://oeis.org/A343713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term of the concatenation of the sorted lists of
 *            distinct residues of triangular numbers T_k = k(k+1)/2 modulo m
 *            for m = 1,2,3,…
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* flattened table */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of entries stored */
static idx_t next_m = 1;         /* next m to process */

/* grow the cache to at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* insufficient memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* append a value to the cache */
static void push_cache(seq_t val)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* process row for a given m, storing its sorted distinct residues */
static void process_row(idx_t m)
{
    /* mark seen residues */
    unsigned char *seen = (unsigned char *)calloc((size_t)m, 1);
    if (!seen)
        abort();

    /* period is 2*m; compute residues for k = 0 … 2*m-1 */
    for (idx_t k = 0; k < (m << 1); ++k)
    {
        /* compute (k*(k+1)/2) mod m */
        unsigned __int128 prod = (unsigned __int128)k * (k + 1ull);
        unsigned long long t = (unsigned long long)((prod / 2ull) % m);
        seen[(size_t)t] = 1;
    }
    /* store residues sorted by ascending value */
    for (idx_t r = 0; r < m; ++r)
        if (seen[(size_t)r])
            push_cache((seq_t)r);

    free(seen);
    next_m++;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, return 0 */

    size_t idx = (size_t)(n - 1u);

    /* extend cache until it contains at least idx+1 elements */
    while (cache_len <= idx)
        process_row(next_m);

    return cache[idx];
}
