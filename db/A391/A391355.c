/*
 * A391355  Numbers k such that 4*k - (greatest prime < 4*k) = (least prime >
 *          4*k) - 4*k.
 * https://oeis.org/A391355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for consecutive primes p and q with (p+q)%8==0, k = (p+q)/8
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   ensure_capacity, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, no term for n==0 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t prev_prime = 0;

    /* initialize on first call */
    if (cache_len == 0) {
        prev_prime = 2;            /* first prime */
    }

    while (cache_len < n) {
        uint64_t next = next_prime(prev_prime);   /* next consecutive prime */
        if (((prev_prime + next) & 7ULL) == 0ULL) {
            seq_t val = (seq_t)(prev_prime + next) / 8ULL;
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = val;
        }
        prev_prime = next;
    }

    return cache[n - 1];
}
