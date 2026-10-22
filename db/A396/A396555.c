/*
 * A396555  Primes p such that EulerPhi(p-1) is not a power of 2.
 * https://oeis.org/A396555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 6507, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, ensure_capacity, is_power_of_two, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation for A396555: primes p such that EulerPhi(p-1) is NOT a power of 2 */
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_power_of_two.h"
#include "../utils/calc_phi.h"
#include "../utils/next_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t cur_prime = 2;        /* last prime processed */

    /* Ensure we have at least n terms in cache (index n-1) */
    while (cache_len < n) {
        /* get next prime */
        cur_prime = next_prime(cur_prime + 1);

        /* compute Euler phi of p-1 */
        seq_t phi = calc_phi((seq_t)(cur_prime - 1));

        /* check condition */
        if (!is_power_of_two(phi)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                /* double capacity or set to 1 if empty */
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cur_prime;
        }
    }
    return cache[n - 1];
}
