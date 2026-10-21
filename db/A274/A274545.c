/*
 * A274545  Values of k such that 5*k-1 and 10*k-1 are both perfect squares.
 * https://oeis.org/A274545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A274545: Values of k such that 5*k-1 and 10*k-1 are both perfect squares.
   Recurrence (from OEIS G.f.): 
      a(n) = a(n-1) + 39202*a(n-2) - 39202*a(n-3) - a(n-4) + a(n-5) for n>5
   Initial values: a(1)=1, a(2)=29, a(3)=33293, a(4)=1130977, a(5)=1305146305
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored */
static bool   initialized = false;

/* Initialise cache with the five known terms. */
static void init(void)
{
    cache_cap = 16;                         /* start with a small capacity */
    cache     = alloc_seq_array(cache_cap);

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)29;
    cache[2] = (seq_t)33293;
    cache[3] = (seq_t)1130977;
    cache[4] = (seq_t)1305146305;
    cache_len = 5;
    initialized = true;
}

/* Grow cache array to at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    seq_t *new_cache = alloc_seq_array(need);
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache)
        free_seq_array(cache);

    cache = new_cache;
    cache_cap = need;
}

/* Return the n‑th term (n ≥ 1) of A274545. */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n < (idx_t)1)
        return (seq_t)0;          /* defined only for n≥1; safety */

    size_t idx = (size_t)n;       /* n fits in size_t for all expected n */

    ensure_capacity(idx);

    /* generate terms up to n using the recurrence */
    while (cache_len < idx)
    {
        /* last five terms: a(n-1)…a(n-5) */
        seq_t t1 = cache[cache_len - 1];   // a(n-1)
        seq_t t2 = cache[cache_len - 2];   // a(n-2)
        seq_t t3 = cache[cache_len - 3];   // a(n-3)
        seq_t t4 = cache[cache_len - 4];   // a(n-4)
        seq_t t5 = cache[cache_len - 5];   // a(n-5)

        /* a(n) = t1 + 39202*t2 - 39202*t3 - t4 + t5 */
        seq_t term = t1;
        term += (seq_t)39202 * t2;
        term -= (seq_t)39202 * t3;
        term -= t4;
        term += t5;

        cache[cache_len] = term;
        ++cache_len;
    }

    return cache[n - 1];   /* array is 0‑based, sequence offset is 1 */
}
