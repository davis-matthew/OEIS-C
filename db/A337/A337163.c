/*
 * A337163  Numbers divisible by their individual digits, but not by the
 *          product of their digits.
 * https://oeis.org/A337163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = numbers divisible by each of their non‑zero digits
   but NOT divisible by the product of those digits.  The function
   generates terms on the fly, memoizing them so that calls out of
   order are efficient. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;   /* cache of found terms          */
static size_t cache_size = 0; /* number of terms stored        */
static size_t cache_cap  = 0; /* allocated capacity of cache   */
static idx_t  next_candidate = 1; /* next integer to test   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence starts at index 1 */

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        seq_t cand = (seq_t)next_candidate;

        seq_t prod  = (seq_t)1;   /* product of non‑zero digits   */
        bool zero_digit = false;
        bool divisible   = true;

        seq_t tmp = cand;
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % (seq_t)10);
            if (d == 0) { zero_digit = true; break; }

            prod *= (seq_t)d;
            if (cand % (seq_t)d != 0) { divisible = false; break; }

            tmp /= (seq_t)10;
        }

        if (!zero_digit && divisible) {
            if (cand % prod != 0) {   /* not divisible by product   */
                grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size] = cand;
                cache_size++;
            }
        }

        next_candidate++;
    }

    return cache[n - 1];
}
