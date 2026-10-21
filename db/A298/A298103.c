/*
 * A298103  The first of five consecutive prime numbers the sum of which is
 *          equal to the sum of five consecutive integers.
 * https://oeis.org/A298103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 326, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Compute the first of five consecutive primes whose sum
   equals the sum of five consecutive integers.  A block of five
   consecutive primes has sum S; it corresponds to a consecutive
   integer block iff S is divisible by 5.  The sequence is the first
   prime in each such block.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Static state */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;

    /* Sieve limit sufficient for the first many terms */
    const size_t SIEVE_LIMIT = 2000000; /* 2 million */

    if (!initialized) {
        /* Build sieve of primes up to SIEVE_LIMIT */
        build_primes_upto(SIEVE_LIMIT, &primes, &primes_cnt);

        /* Allocate a bit of storage for term list */
        terms_cap = 256;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) {
            /* allocation failure: cannot proceed */
            return 0;
        }

        /* Find all qualifying blocks in the prime list */
        if (primes_cnt >= 5) {
            seq_t sum = 0;
            /* Initial window of first five primes */
            for (size_t i = 0; i < 5; ++i)
                sum += (seq_t)primes[i];

            for (size_t i = 0; i + 5 <= primes_cnt; ++i) {
                /* Check divisibility by 5 */
                if (((seq_t)sum % 5) == 0) {
                    /* Grow term buffer if necessary */
                    if (terms_cnt == terms_cap) {
                        terms_cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                        if (!tmp) {
                            free(terms);
                            return 0;
                        }
                        terms = tmp;
                    }
                    terms[terms_cnt++] = (seq_t)primes[i];
                }

                /* Slide window: subtract lowest, add next */
                sum -= (seq_t)primes[i];
                if (i + 5 < primes_cnt)
                    sum += (seq_t)primes[i + 5];
            }
        }

        initialized = true;
    }

    /* Bounds check; return 0 if n is out of range */
    if (n <= 0 || (idx_t)n > terms_cnt)
        return 0;

    return terms[(size_t)n - 1];
}
