/*
 * A360490  a(n) = (1/2) * A241102(n).
 * https://oeis.org/A360490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * A241102(n) (primes obtained as half the
 *            difference * of two cubes of primes). This implementation
 *            generates twin prime * pairs (p, p+2), computes m = p+1, forms
 *            the candidate * 3*m^2 + 1, and keeps it if it is prime. Results
 *            are cached so that * calls in any order are fast
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* cached terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* the next odd number to test as the first of a twin prime pair */
    static uint64_t candidate = 3;

    /* grow cache until the n-th term is available */
    while (cache_len < (size_t)n) {
        /* look for the next twin prime pair (p, p+2) */
        uint64_t p = 0, q = 0;
        while (true) {
            /* ensure candidate stays odd */
            if ((candidate & 1ULL) == 0)
                candidate++;

            /* test candidate and candidate+2 for primality */
            if (!is_prime_big((seq_t)candidate)) {
                candidate += 2;
                continue;
            }
            uint64_t cand_q = candidate + 2;
            if (!is_prime_big((seq_t)cand_q)) {
                candidate += 2;
                continue;
            }

            /* twin prime pair found */
            p = candidate;
            q = cand_q;
            /* next search should start at q for the following pair */
            candidate = cand_q;
            break;
        }

        /* compute the candidate value 3*(p+1)^2 + 1 */
        uint64_t m = p + 1;
        seq_t m_t  = (seq_t)m;
        seq_t val  = (seq_t)3 * m_t * m_t + (seq_t)1;

        /* if the value is prime, store it */
        if (is_prime_big(val)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = alloc_seq_array(new_cap);
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                if (cache != NULL)
                    free_seq_array(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)(n - 1)];
}
