/*
 * A281054  Number of nX6 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   empirical recurrence a(n) = 3*a(n-1) + 3*a(n-2) - 11*a(n-3) -
 *            9*a(n-4) + 15*a(n-5) + 20*a(n-6) + 3*a(n-7) - 18*a(n-8) -
 *            27*a(n-9) - 9*a(n-10) + 15*a(n-11) + 20*a(n-12) + 12*a(n-13) -
 *            7*a(n-15) - 6*a(n-16) - 3*a(n-17) - a(n-18) for n > 21, with the
 *            first 21 terms given below
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 20, u64 n <= 55, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

static const seq_t init_vals[21] = {
    (seq_t)6,          /* a(1) */
    (seq_t)251,        /* a(2) */
    (seq_t)1403,       /* a(3) */
    (seq_t)5400,       /* a(4) */
    (seq_t)16875,      /* a(5) */
    (seq_t)46610,      /* a(6) */
    (seq_t)119205,     /* a(7) */
    (seq_t)288527,     /* a(8) */
    (seq_t)671451,     /* a(9) */
    (seq_t)1515524,    /* a(10) */
    (seq_t)3339671,    /* a(11) */
    (seq_t)7217269,    /* a(12) */
    (seq_t)15347530,   /* a(13) */
    (seq_t)32196671,   /* a(14) */
    (seq_t)66765699,   /* a(15) */
    (seq_t)137073842,  /* a(16) */
    (seq_t)278977066,  /* a(17) */
    (seq_t)563443975,  /* a(18) */
    (seq_t)1130260537, /* a(19) */
    (seq_t)2253558987, /* a(20) */
    (seq_t)4468793135  /* a(21) */
};

/* initialise cache with the first 21 terms */
static void init_cache(void)
{
    if (cache != NULL) return;

    cache_cap   = 32;
    cache_size  = 21;
    cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));

    for (idx_t i = 0; i < cache_cap; ++i)
        cache[i] = 0;          /* clear all slots */

    for (idx_t i = 0; i < 21; ++i)
        cache[i] = init_vals[i];
}

/* extend cache up to and including index n-1 (0‑based) */
static void ensure_cache(idx_t n)
{
    init_cache();

    if (n <= cache_size) return;          /* already computed */

    /* grow the allocated array if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t idx = cache_size; idx < n; ++idx) {
        seq_t term = 0;
        term += 3 * cache[idx - 1];
        term += 3 * cache[idx - 2];
        term -= 11 * cache[idx - 3];
        term -= 9  * cache[idx - 4];
        term += 15 * cache[idx - 5];
        term += 20 * cache[idx - 6];
        term += 3  * cache[idx - 7];
        term -= 18 * cache[idx - 8];
        term -= 27 * cache[idx - 9];
        term -= 9  * cache[idx - 10];
        term += 15 * cache[idx - 11];
        term += 20 * cache[idx - 12];
        term += 12 * cache[idx - 13];
        /* no term for idx-14 (coeff 0) */
        term -= 7  * cache[idx - 15];
        term -= 6  * cache[idx - 16];
        term -= 3  * cache[idx - 17];
        term -=      cache[idx - 18];
        cache[idx] = term;
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is indexed from 1 */
    ensure_cache(n);
    return cache[n - 1];
}
