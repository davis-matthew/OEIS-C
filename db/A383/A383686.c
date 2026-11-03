/*
 * A383686  The greater of two consecutive primes whose gap equals the
 *          difference between their digital roots.
 * https://oeis.org/A383686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   digit_frequencies, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the larger of two consecutive primes p<q
   for which the prime gap q-p equals the difference of their digital
   roots:  q - p = digital_root(q) - digital_root(p). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* External utility functions provided by the framework.  */
#include <stdlib.h>
#include "../utils/digit_frequencies.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static void seq_free(seq_t *p) { mem_free(p); }

/* Compute the digital root of a value of type seq_t.  */
static int digital_root(seq_t v)
{
    if (v == 0) return 0;
    int sum = 0;
    seq_t t = v;
    while (t != 0) {
        sum += (int)(t % 10);
        t /= 10;
    }
    int r = sum % 9;
    return r ? r : 9;
}

/* Return the n-th term of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_prime = 0;

    /* Initialise on first call. */
    if (!ready) {
        cache_cap = 64;
        cache     = seq_alloc(cache_cap);   /* allocate array of seq_t */
        cache_len = 0;
        last_prime = 2;                     /* start with the first prime */
        ready = true;
    }

    /* Generate terms until we reach the requested index. */
    while (cache_len < (size_t)n) {
        uint64_t p = last_prime;
        uint64_t q = next_prime(p);         /* next prime after p */

        int dr_p = digital_root((seq_t)p);
        int dr_q = digital_root((seq_t)q);
        seq_t gap = (seq_t)q - (seq_t)p;

        /* Only gaps up to 8 can satisfy dr_q - dr_p == gap. */
        if (gap <= 8) {
            int diff = dr_q - dr_p;
            if (diff == (int)gap) {
                /* Make room if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = seq_alloc(new_cap);
                    mem_copy(tmp, cache, cache_len * sizeof(seq_t));
                    seq_free(cache);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)q;
            }
        }

        last_prime = q;                     /* advance to next left prime */
    }

    return cache[n - 1];
}
