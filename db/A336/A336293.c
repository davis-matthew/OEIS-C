/*
 * A336293  a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(2*k,k) * (n-k)!.
 * https://oeis.org/A336293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(2*k,k) * (n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n) = (3*n^2 + n - 1)*a(n-1) - (n-1)^2*(3*n + 1)*a(n-2) 
   + (n-2)^2*(n-1)^2*a(n-3); with a(0)=1, a(1)=3, a(2)=16. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_cap = 0;

/* Grow the cache to accommodate index n (0‑based). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap : 1;
    while (new_cap <= n + 1)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    seq_cache = tmp;
    seq_cache_cap = new_cap;
    /* Initialise new slots to 0 so we can test whether a value was set. */
    for (size_t i = n + 1; i < new_cap; ++i)
        seq_cache[i] = 0;
}

/* Return a(n).  Results are memoised across calls. */
seq_t a(idx_t n)
{
    /* Basic sanity: n must be <= 1 should produce 1,3 as defined. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)3;

    ensure_cache(n);

    /* Initialise known seeds. */
    if (seq_cache[0] == 0) seq_cache[0] = (seq_t)1;
    if (seq_cache[1] == 0) seq_cache[1] = (seq_t)3;
    if (seq_cache[2] == 0) seq_cache[2] = (seq_t)16;

    for (idx_t i = 3; i <= n; ++i) {
        if (seq_cache[i] != 0)
            continue; /* already computed */

        /* Use 128‑bit intermediate arithmetic when seq_t is an
           unsigned integer type so that intermediate products do
           not overflow, even when the final result fits into seq_t. */
#if defined(__SIZEOF_INT128__)
        unsigned __int128 term1 = (unsigned __int128)(3 * i * i + i - 1)
                                 * (unsigned __int128)seq_cache[i - 1];
        unsigned __int128 term2 = (unsigned __int128)(i - 1) * (i - 1)
                                 * (unsigned __int128)(3 * i + 1)
                                 * (unsigned __int128)seq_cache[i - 2];
        unsigned __int128 term3 = (unsigned __int128)(i - 2) * (i - 2)
                                 * (unsigned __int128)(i - 1) * (i - 1)
                                 * (unsigned __int128)seq_cache[i - 3];
        unsigned __int128 numer = term1 - term2 + term3;
        seq_cache[i] = (seq_t)(numer / i);
#else
        /* If 128‑bit types are not available we rely on seq_t
           itself to provide the necessary range (e.g. for arbitrary‑precision). */
        seq_t term1 = (seq_t)(3 * i * i + i - 1) * seq_cache[i - 1];
        seq_t term2 = (seq_t)(i - 1) * (i - 1) * (seq_t)(3 * i + 1) * seq_cache[i - 2];
        seq_t term3 = (seq_t)(i - 2) * (i - 2) * (seq_t)(i - 1) * (i - 1) * seq_cache[i - 3];
        seq_t numer = term1 - term2 + term3;
        seq_cache[i] = numer / i;
#endif
    }

    return seq_cache[n];
}
