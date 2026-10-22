/*
 * A351102  Variation of the Sisyphus sequence A350877: the same rules apply
 *          except that each time a(n) is divided by a prime the dividing
 *          prime is incremented to the next prime.
 * https://oeis.org/A351102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A351102):
 * a(1) = 1.
 * Maintain two primes:
 *   divPrime – current divisor prime (initially 2)
 *   addPrime – current additive prime (initially 2)
 * Maintain a flag prev_div indicating whether the previous term
 * was produced by a division.
 *
 * For n > 1 let x = a(n‑1):
 *   If prev_div is true:
 *      a(n) = x + addPrime;
 *      addPrime = next_prime(addPrime);
 *      prev_div = false;
 *   else if x % divPrime == 0:
 *      divide x by divPrime repeatedly:
 *          while (x % divPrime == 0) x /= divPrime;
 *      a(n) = x;
 *      divPrime = next_prime(divPrime);
 *      prev_div = true;
 *   else:
 *      a(n) = x + addPrime;
 *      addPrime = next_prime(addPrime);
 *      prev_div = false;
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t divPrime = 2;
    static uint64_t addPrime = 2;
    static bool prev_div     = false;

    if (n < 1) {
        /* n < 1 is outside the defined offset; return 0 (undefined) */
        return 0;
    }

    /* Initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = (seq_t)1;            /* a(1) */
        cache_len = 1;
        divPrime = 2;
        addPrime = 2;
        prev_div = false;
    }

    /* Grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];     /* a(i) is stored at cache[i-1] */
        seq_t next;

        if (prev_div) {                /* previous term came from a division */
            next = prev + (seq_t)addPrime;
            addPrime = next_prime(addPrime);
            prev_div = false;
        } else {
            if (prev % divPrime == 0) {   /* division case */
                seq_t temp = prev;
                while (temp % divPrime == 0) {
                    temp /= divPrime;
                }
                next = temp;
                divPrime = next_prime(divPrime);
                prev_div = true;
            } else {                      /* addition case */
                next = prev + (seq_t)addPrime;
                addPrime = next_prime(addPrime);
                prev_div = false;
            }
        }
        cache[i] = next;
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
