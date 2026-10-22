/*
 * A321178  One-half the sum of the first 2*n + 1 primes.
 * https://oeis.org/A321178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p₁ + p₂ + … + p_{2n+1}) / 2 where p₁,p₂,… are the primes
 *            in increasing order
 * Range:     u8 n <= 6, u16 n <= 106, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Number of primes required for this n */
    idx_t k = ((idx_t)2 * n) + 1;          /* k >= 1 for all n >= 0 */

    /* Static cache for primes and their cumulative sums */
    static uint64_t *prime_arr  = NULL;    /* primes themselves   */
    static seq_t   *prefix     = NULL;    /* prefix sums of primes */
    static size_t   prime_cnt  = 0;        /* how many primes cached */
    static size_t   prime_cap  = 0;        /* capacity of the above arrays */
    static uint64_t next_candidate = 3;    /* next odd number to test */

    /* Ensure cache capacity */
    if (prime_cap == 0) {
        size_t init_cap = 16;
        prime_arr = (uint64_t *)realloc(prime_arr, init_cap * sizeof(uint64_t));
        prefix    = (seq_t *)realloc(prefix,    init_cap * sizeof(seq_t));
        prime_cap = init_cap;
    }

    /* Initialise with the first prime (2) if cache empty */
    if (prime_cnt == 0) {
        prime_arr[0]    = 2;
        prefix[0]       = (seq_t)2;
        prime_cnt       = 1;
        next_candidate  = 3;            /* start with the next odd candidate */
    }

    /* Generate primes until we have at least k primes */
    while (prime_cnt < (size_t)k) {
        uint64_t p = next_candidate;
        bool is_prime = true;

        /* Check divisibility by known primes up to √p */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = prime_arr[i];
            if ((unsigned __int128)q * q > p) break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Grow arrays if needed */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap * 2;
                prime_arr = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
                prefix    = (seq_t *)realloc(prefix,    new_cap * sizeof(seq_t));
                prime_cap = new_cap;
            }

            prime_arr[prime_cnt] = p;
            prefix[prime_cnt] = prefix[prime_cnt - 1] + (seq_t)p;
            ++prime_cnt;
        }

        /* Advance to next odd candidate */
        next_candidate += 2;
    }

    /* Return the half‑sum of the first k primes */
    return prefix[k - 1] / 2;
}
