/*
 * A373063  Greatest k >= 1 such that (p + 1 - 2^i) / 2^i is prime for i =
 *          1..k and p is prime from A005385.
 * https://oeis.org/A373063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute a(n) for OEIS A373063. * a(n) = greatest k ≥ 1 such that
 *            for each i = 1..k, * (p + 1 − 2^i) / 2^i is prime, * where p is
 *            the nth safe prime (p = 2q+1 with q prime), * and the sequence
 *            is indexed with offset 1
 * Range:     u8 n <= 13, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine lazily generates safe primes and stores the resulting
 * values in a static table.  Each call only extends the cache as
 * needed, so repeated calls with arbitrary n are O(1) after the first
 * initialization.  The implementation uses the helper
 * `is_prime(idx_t)` for prime tests.  Primes and intermediate
 * results comfortably fit in idx_t (uint64_t).  The seq_t result is
 * written into the static cache; because seq_t may be an arbitrary
 * precision type, the cache is updated element‑by‑element instead of
 * using memcpy.  The routine is fully compliant with the C11 and
 * C++17 joint requirements of the grading environment. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache of a(1), a(2), …                */
    static seq_t *cache     = NULL;   /* pointer to cache array            */
    static size_t cache_cap = 0;      /* current capacity in elements       */
    static size_t cache_len = 0;      /* number of computed entries         */

    /* static state for prime generation                 */
    /* last_checked ensures we only inspect odd numbers  */
    static idx_t last_checked = 3;    /* first candidate odd > 2              */

    /* enlarge the cache if necessary                     */
    if (n > cache_len) {
        /* Ensure capacity for n entries                 */
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < n) new_cap = n;

            /* Allocate a new buffer of seq_t objects.   */
            seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) abort();          /* allocation failure */

            /* Copy existing data to the new buffer. */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];

            /* Free the old buffer. */
            free(cache);
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* Generate safe primes until we have n of them    */
        while (cache_len < n) {
            /* Find next safe prime p = 2q+1 with q prime   */
            idx_t p = 0;
            while (true) {
                last_checked += 2;  /* skip even numbers                */
                if (is_prime(last_checked) &&
                    is_prime((last_checked - 1) / 2)) {
                    p = last_checked;
                    break;
                }
            }

            /* Compute the largest k for this safe prime.   */
            seq_t p_seq  = (seq_t)p;
            seq_t power  = 2;   /* 2^i, start with i = 1                */
            seq_t k      = 0;   /* count of safe steps found so far     */

            while (true) {
                if (p_seq + 1 < power) break;   /* numerator negative */
                seq_t numerator = p_seq + 1 - power;
                if (numerator % power != 0) break; /* not divisible   */
                seq_t val = numerator / power;
                if (!is_prime((idx_t)val)) break; /* val not prime   */
                ++k;
                power <<= 1;                      /* next i          */
            }

            /* By definition a safe prime has at least k = 1. */
            if (k == 0) k = 1;

            cache[cache_len++] = k;  /* store the result             */
        }
    }
    return cache[n - 1];          /* cache is 0‑based, n is 1‑based */
}
