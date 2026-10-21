/*
 * A285689  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of four primes.
 * https://oeis.org/A285689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n+1) = smallest prime > a(n) such that a(n+1)-a(n)
 *            is product of four primes
 * Range:     u8 n <= 8, u16 n <= 1402, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, grow_seq, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0;   /* undefined for n<1 */

    if (cache == NULL || cache_size == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)2;
        cache_size = 1;
    }

    while (cache_size < n) {
        seq_t prev = cache[cache_size - 1];
        seq_t cand = (seq_t)next_prime((uint64_t)prev);
        while (1) {
            seq_t diff = cand - prev;
            uint64_t d     = (uint64_t)diff;
            int count      = 0;
            while (d > 1 && count <= 4) {
                uint64_t spf = _smallest_prime_factor(d);
                if (spf == 0) spf = d;   /* fallback for safety */
                while (d % spf == 0) {
                    d /= spf;
                    ++count;
                    if (count > 4) break;
                }
                if (count > 4) break;
            }
            if (count == 4) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size] = cand;
                ++cache_size;
                break;
            }
            cand = (seq_t)next_prime((uint64_t)cand);
        }
    }
    return cache[n - 1];
}
