/*
 * A345925  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum 2.
 * https://oeis.org/A345925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by searching for k whose k‑th composition (in standard
   order) has alternating sum equal to 2.  Results are cached for
   subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Cache for found k values ---------- */
static seq_t *cache    = NULL;   /* array of found k values             */
static size_t cache_cap = 0;     /* allocated capacity of cache         */
static size_t cache_size = 0;    /* number of k values stored           */
static idx_t  max_k_done  = 0;   /* largest k that has been tested      */

/* Ensure cache has space for at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    /* realloc returns void*; cast explicitly for C++ */
    seq_t *new_p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_p) /* memory allocation failed */
        abort();
    cache = new_p;
    cache_cap = new_cap;
}

/* Compute the n-th term of sequence A345925 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1 */

    /* Generate terms until we have at least n values */
    while (cache_size < (size_t)n)
    {
        /* Test next integer k = max_k_done + 1 */
        ++max_k_done;
        idx_t k = max_k_done;

        /* Extract positions of 1 bits (1‑based) in reversed binary expansion of k */
        unsigned int pos[128];      /* positions of set bits */
        unsigned int diff[128];     /* differences between consecutive positions */
        size_t m = 0;               /* number of set bits */

        idx_t tmp = k;
        unsigned int i = 0;         /* bit index starting at 0 */
        while (tmp)
        {
            if (tmp & 1)
                pos[m++] = i + 1;    /* positions are 1‑based */
            ++i;
            tmp >>= 1;
        }

        /* Compute the differences and their alternating sum in reverse order */
        unsigned int prev = 0;
        for (size_t j = 0; j < m; ++j)
        {
            diff[j] = pos[j] - prev;
            prev    = pos[j];
        }

        long long sum = 0;
        int sign = 1;                /* positive for the first part in the reversed list */
        for (size_t j = m; j > 0; --j)
        {
            sum += (long long)sign * (long long)diff[j - 1];
            sign = -sign;
        }

        /* If alternating sum equals 2, store k in cache */
        if (sum == 2)
        {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)k;
        }
    }

    /* Return the n‑th stored k (1‑based indexing) */
    return cache[n - 1];
}
