/*
 * A348594  Numbers m such that m^2 + 1 = p*q with p, q primes and m = (p +
 *          q)/2 - 1.
 * https://oeis.org/A348594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A348594: m such that m^2 + 1 = p*q with p,q primes and m = (p + q)/2 - 1.
   The equal form is m = 2*t^2 where both 2*t^2 - 2*t + 1 and 2*t^2 + 2*t + 1 are prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Two‑level cache: already generated terms and the next t to test. */
    static seq_t *cache = NULL;     /* array of found m values */
    static size_t cache_len = 0;    /* number of values stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t next_t = 2;        /* start with t = 2 → m = 8 */

    /* Generate terms until we have the n‑th one. */
    while (cache_len < n) {
        idx_t t = next_t++;                  /* next t to examine, as idx_t */
        seq_t st = (seq_t)t;                 /* promote to seq_t for arithmetic */

        /* p = 2*t^2 - 2*t + 1
           q = 2*t^2 + 2*t + 1                        */
        seq_t p = (seq_t)2 * st * st - (seq_t)2 * st + (seq_t)1;
        seq_t q = (seq_t)2 * st * st + (seq_t)2 * st + (seq_t)1;

        if (is_prime_big(p) && is_prime_big(q)) {
            /* m = 2*t^2                                       */
            seq_t m = (seq_t)2 * st * st;

            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* OOM – acceptable in OEIS test harness */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = m;
        }
    }
    return cache[n - 1];   /* OEIS indices start at 1 */
}
