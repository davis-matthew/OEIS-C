/*
 * A359772  Union of even numbers with an even number of prime factors and odd
 *          numbers with an odd number of prime factors, when the number of
 *          prime factors is counted with multiplicity.
 * https://oeis.org/A359772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th number in the union of * (even numbers with an
 *            even number of prime factors) and * (odd numbers with an odd
 *            number of prime factors), * counted with multiplicity. * The
 *            function builds the sequence lazily using a sieve of * smallest
 *            prime factors and a growing cache
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint32_t *g_spf = NULL;          /* smallest prime factor array   */
static size_t   g_spf_limit = 0;        /* current maximum index of spf  */
static seq_t   *g_cache = NULL;         /* cache of sequence terms        */
static size_t   g_cache_cap = 0;        /* capacity of cache             */
static size_t   g_cache_len = 0;        /* number of terms cached         */
static idx_t    g_last_checked = 0;     /* last integer examined          */

/* Rebuild the smallest‑prime‑factor array up to 'limit'. */
static void compute_spf(size_t limit)
{
    if (g_spf) free(g_spf);
    g_spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!g_spf) abort();
    for (size_t i = 0; i <= limit; ++i) g_spf[i] = (uint32_t)i;
    g_spf[0] = 0;
    if (limit >= 1) g_spf[1] = 1;

    for (size_t i = 2; (i * i) <= limit; ++i) {
        if (g_spf[i] == (uint32_t)i) {          /* i is prime            */
            for (size_t j = i * i; j <= limit; j += i) {
                if (g_spf[j] == (uint32_t)j)     /* first encounter      */
                    g_spf[j] = (uint32_t)i;
            }
        }
    }
    g_spf_limit = limit;
}

/* Count the number of prime factors (with multiplicity) of n
 * using the precomputed spf array. */
static unsigned int omega_with_spf(idx_t n)
{
    if (n <= 1) return 0;
    unsigned int cnt = 0;
    while (n > 1) {
        ++cnt;
        n /= g_spf[(size_t)n];
    }
    return cnt;
}

/* Decide whether n satisfies the sequence condition. */
static inline bool desired(idx_t n)
{
    if (n <= 1) return false;
    unsigned int omega = omega_with_spf(n);
    bool evenOmega = (omega & 1) == 0;      /* parity of Ω(n)    */
    bool evenN     = (n & 1) == 0;          /* parity of n       */
    return (evenN && evenOmega) || (!evenN && !evenOmega);
}

/* Ensure the sieve covers at least 'needed' indices. */
static void ensure_spf_capacity(size_t needed)
{
    if (needed <= g_spf_limit) return;
    size_t new_limit = g_spf_limit ? g_spf_limit * 2 : 1024;
    while (new_limit < needed) new_limit *= 2;
    compute_spf(new_limit);
}

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= g_cache_cap) return;
    size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(g_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    g_cache   = new_cache;
    g_cache_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset – safe fallback */

    while (g_cache_len < n) {
        /* Extend sieve if we are about to look beyond its range */
        if (g_last_checked >= g_spf_limit)
            ensure_spf_capacity(g_last_checked + 1024);

        ++g_last_checked;               /* advance candidate number */
        if (!desired(g_last_checked))   /* skip non‑matching integers */
            continue;

        ensure_cache_capacity(g_cache_len + 1);
        g_cache[g_cache_len++] = (seq_t)g_last_checked;
    }

    return g_cache[n - 1];
}
