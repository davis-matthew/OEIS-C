/*
 * A348307  Primes p such that (p-1)/2, (p-2)/3, 2*p+1, 3*p+2 are all prime
 *          numbers.
 * https://oeis.org/A348307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A348307: primes p such that (p-1)/2, (p-2)/3, 2p+1, 3p+2 are all prime */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Cache of found terms – 1‑indexed in the array */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Candidate q running over primes ≡ 2 (mod 3), i.e. safe primes q = (p‑1)/2 */
    static uint64_t next_q = 3; /* start from the first odd prime */

    if (n == 0)
        return (seq_t)0; /* out of range, but not used */

    while (cache_len < n) {
        /* Only consider q ≡ 2 (mod 3); otherwise p = 2q+1 would not satisfy (p‑2)/3 integer */
        if ((next_q % 3ULL) == 2ULL) {
            seq_t q = (seq_t)next_q;
            seq_t p = (q << 1) + 1;            /* p = 2q + 1 */
            if (is_prime_big(p)) {
                /* (p-1)/2 is already q (prime) */
                seq_t r = (p - 2) / 3;        /* (p-2)/3 */
                if (is_prime_big(r)) {
                    seq_t two_p1 = (p << 1) + 1;                 /* 2p + 1 */
                    if (is_prime_big(two_p1)) {
                        seq_t three_p2 = (p << 1) + p + 2;       /* 3p + 2 */
                        if (is_prime_big(three_p2)) {
                            /* Grow cache if necessary */
                            if (cache_len >= cache_cap) {
                                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                                cache = seq_realloc(cache, cache_cap, new_cap);
                                cache_cap = new_cap;
                            }
                            cache[cache_len++] = p;
                        }
                    }
                }
            }
        }
        /* Advance to the next prime q */
        next_q = next_prime(next_q + 1);
    }

    return cache[n - 1];
}
