/*
 * A279141  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 137", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A279141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + a(n-3) + a(n-4) + 100000000*a(n-8) +
 *            100000000*a(n-9) - 100000000*a(n-11) - 100000000*a(n-12) for n >
 *            15 Initial values: a(0)=1, a(1)=0, a(2)=101, a(3)=0, a(4)=10101,
 *            a(5)=0, a(6)=1010101, a(7)=0, a(8)=101011101, a(9)=10100,
 *            a(10)=10101000001, a(11)=11100, a(12)=1010111010101, a(...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Base values for n = 0 … 15.  These are the decimal numbers whose
   decimal digits form the binary representation of the CA x‑axis. */
static const seq_t base_terms[16] = {
    (seq_t)1,                     /* a(0)  */
    (seq_t)0,                     /* a(1)  */
    (seq_t)101,                   /* a(2)  */
    (seq_t)0,                     /* a(3)  */
    (seq_t)10101,                 /* a(4)  */
    (seq_t)0,                     /* a(5)  */
    (seq_t)1010101,               /* a(6)  */
    (seq_t)0,                     /* a(7)  */
    (seq_t)101011101,             /* a(8)  */
    (seq_t)10100,                 /* a(9)  */
    (seq_t)10101000001,           /* a(10) */
    (seq_t)11100,                 /* a(11) */
    (seq_t)1010111010101,         /* a(12) */
    (seq_t)101000000,             /* a(13) */
    (seq_t)101010000010101,       /* a(14) */
    (seq_t)111000000               /* a(15) */
};

/* Dynamic cache of computed values.  cache[0 .. cache_len-1] are valid. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)  /* already computed */
        return;

    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)      /* allocation failed – leave old cache */
        return;
    cache = new_cache;

    /* Compute new entries. */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i < 16) {
            cache[i] = base_terms[(size_t)i];
        } else {
            /* Use a 128‑bit intermediate when seq_t is a builtin
               unsigned integer type (size ≤ 8 bytes).  For larger
               types the builtin multiplication will be handled by
               the type’s arithmetic operators. */
            if (sizeof(seq_t) <= 8) {
                __int128_t term3  = (__int128_t)cache[i-3];
                __int128_t term4  = (__int128_t)cache[i-4];
                __int128_t term8  = (__int128_t)cache[i-8] * 100000000ULL;
                __int128_t term9  = (__int128_t)cache[i-9] * 100000000ULL;
                __int128_t term1  = (__int128_t)cache[i-1];
                __int128_t term11 = (__int128_t)cache[i-11] * 100000000ULL;
                __int128_t term12 = (__int128_t)cache[i-12] * 100000000ULL;
                __int128_t val = term3 + term4 + term8 + term9
                                 - term1 - term11 - term12;
                cache[i] = (seq_t)val;
            } else {
                seq_t term8  = cache[i-8] * (seq_t)100000000ULL;
                seq_t term9  = cache[i-9] * (seq_t)100000000ULL;
                seq_t term11 = cache[i-11] * (seq_t)100000000ULL;
                seq_t term12 = cache[i-12] * (seq_t)100000000ULL;

                seq_t val = cache[i-3] + cache[i-4] + term8 + term9;
                val -= cache[i-1];
                val -= term11;
                val -= term12;

                cache[i] = val;
            }
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 16)
        return base_terms[(size_t)n];

    ensure_cache(n);
    return cache[(size_t)n];
}
