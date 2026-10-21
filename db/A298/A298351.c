/*
 * A298351  a(n) = a(n-1) + a(n-2) + 2 a(ceiling(n/2)), where a(0) = 1, a(1) =
 *          2, a(2) = 3.
 * https://oeis.org/A298351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(ceil(n/2)), a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 40, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_size = 0;

/* Ensure storage for indices up to n (inclusive). */
static void a_ensure_cap(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (need <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap : 4;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* terminate on allocation failure */
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* Compute all terms up to and including n, filling the cache. */
static void a_compute_up_to(idx_t n)
{
    if ((size_t)(n + 1) <= a_cache_size) return;
    a_ensure_cap(n);
    for (idx_t i = a_cache_size; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)            val = 1;
        else if (i == 1)       val = 2;
        else if (i == 2)       val = 3;
        else
        {
            idx_t half = (i + 1) >> 1;   /* ceil(i/2) */
            val = a_cache[i-1] + a_cache[i-2] + a_cache[half] * 2;
        }
        a_cache[i] = val;
    }
    a_cache_size = n + 1;
}

seq_t a(idx_t n)
{
    a_compute_up_to(n);
    return a_cache[n];
}
