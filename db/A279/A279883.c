/*
 * A279883  Primes of the form (prime(j)-1)^(prime(j)+1) + 1.
 * https://oeis.org/A279883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow, get_prime, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A279883: primes of the form (prime(j)-1)^(prime(j)+1)+1; 
   The function iterates through successive primes, computes the candidate 
   (p-1)^(p+1)+1, tests it for primality and records primes that pass. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* stored sequence values */
    static size_t cap = 0;          /* allocated capacity */
    static size_t size = 0;         /* number of stored values */
    static idx_t prime_idx = 1;     /* next prime index to test */

    /* Grow the cache until it contains at least 'n' terms */
    while (size < n) {
        /* Get the j-th prime number */
        uint32_t p = get_prime(prime_idx++);
        /* Compute (p-1)^(p+1) + 1 */
        seq_t base = (seq_t)(p - 1);
        seq_t exp  = (seq_t)(p + 1);
        seq_t val  = big_pow(base, exp);
        val = val + 1;

        /* Keep it if the candidate itself is prime */
        if (is_prime_big(val)) {
            grow_seq(&cache, &cap, size + 1);
            cache[size++] = val;
        }
    }
    return cache[n - 1];
}
