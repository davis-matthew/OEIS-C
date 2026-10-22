/*
 * A344825  Integers whose digit sum is prime and whose digit product is a
 *          perfect square > 0.
 * https://oeis.org/A344825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344825: integers whose digit sum is prime and whose digit product is a perfect square > 0, enumerated in increasing order.
   This implementation searches numbers in order and caches the results to answer arbitrary queries in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_ll.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* offset is 1 */

    static seq_t *cache = NULL;                    /* stored terms */
    static idx_t cache_len = 0;                    /* number of stored terms */
    static idx_t cache_cap = 0;                    /* allocated capacity */
    static seq_t next_candidate = 1;               /* next integer to test */

    while (cache_len < n) {
        seq_t num = next_candidate++;
        seq_t temp = num;
        bool has_zero = false;
        unsigned char mask = 0;                    /* parity bits for 2,3,5,7 */
        long long sum = 0;                         /* digit sum */

        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 10);
            if (digit == 0) { has_zero = true; break; }
            sum += digit;

            /* Update parity of prime exponents in the digit product */
            switch (digit) {
                case 2: case 8:  mask ^= 1; break;   /* 2^odd -> bit 0 */
                case 3:          mask ^= 2; break;   /* 3^odd -> bit 1 */
                case 6:          mask ^= 3; break;   /* 2^odd and 3^odd -> bits 0 & 1 */
                case 5:          mask ^= 4; break;   /* 5^odd  -> bit 2 */
                case 7:          mask ^= 8; break;   /* 7^odd  -> bit 3 */
                /* digits 1,4,9 contribute even exponents => no change */
                default: break;
            }

            temp /= 10;
        }

        if (has_zero)             continue; /* product would be 0 */
        if (mask != 0)             continue; /* product not a perfect square */
        if (!is_prime_ll(sum))     continue; /* digit sum not prime */

        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_len++] = num;
    }

    return cache[n - 1];
}
