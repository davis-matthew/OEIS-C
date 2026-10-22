/*
 * A322517  Primes of form 1357913579... .
 * https://oeis.org/A322517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime obtained from successive prefixes of the
 *            repeating decimal 1357913579
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Helpers:   alloc_seq, ensure_capacity, free_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined offset, return 0 for safety */

    /* static state shared between calls */
    static seq_t *prime_cache = NULL;  /* array of found primes       */
    static idx_t  cache_size  = 0;     /* number of stored primes     */
    static size_t cache_cap   = 0;     /* allocated capacity of cache */
    static seq_t  current     = 0;     /* current prefix value        */
    static idx_t  len         = 0;     /* number of digits processed  */
    static idx_t  digit_pos   = 0;     /* position within the pattern */
    const int pattern[5] = {1, 3, 5, 7, 9};

    /* initialise cache once */
    if (prime_cache == NULL) {
        cache_cap   = 16;
        prime_cache = alloc_seq(cache_cap);
    }

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        int digit = pattern[digit_pos % 5];
        digit_pos++;
        len++;
        current = current * 10 + (seq_t)digit;

        /* start looking for primes only after two digits */
        if (len >= 2 && is_prime_big(current)) {
            if ((size_t)cache_size >= cache_cap) {
                /* grow the cache to accommodate one more prime */
                ensure_capacity((size_t)(cache_size + 1), &cache_cap,
                               (void **)&prime_cache, sizeof(seq_t));
            }
            prime_cache[cache_size++] = current;
        }
    }

    return prime_cache[n - 1];
}
