/*
 * A266234  Primes representable as f(f(f(...f(p)...))) where p is a prime and
 *          f(x) = x*2 - 1.
 * https://oeis.org/A266234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p for which there exists k ≥ 1 such that
 *            (p + 2^k – 1)/2^k is prime. This implementation searches primes
 *            in ascending order until n terms have been found
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* A generous limit that guarantees at least 10 000 terms.
       The prime counting function π(2 000 000) ≈ 148 933, well above 10 000. */
    const idx_t LIMIT = 2000000;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (primes == NULL) {
        build_primes_upto(LIMIT, &primes, &prime_cnt);
    }

    size_t found = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* Skip 2 because it is not represented with a prime ancestor. */
        if (p < 3) continue;

        bool ok = false;
        uint64_t pk = 2;           /* 2^k for k = 1 */
        uint64_t p_minus_one = p - 1;
        while (pk <= p_minus_one) {
            uint64_t numerator = p + pk - 1;
            if ((numerator % pk) == 0) {
                uint64_t y = numerator / pk;
                if (is_prime(y)) {
                    ok = true;
                    break;
                }
            }
            /* Prevent overflow of pk << 1 */
            if (pk > (uint64_t)-1 / 2) break;
            pk <<= 1;
        }

        if (ok) {
            ++found;
            if (found == n)
                return (seq_t)p;
        }
    }

    /* In the unlikely event the limit is too small, extend and retry. */
    idx_t cur_limit = LIMIT * 2;
    while (found < n) {
        free(primes);
        build_primes_upto(cur_limit, &primes, &prime_cnt);
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p < 3) continue;
            bool ok = false;
            uint64_t pk = 2;
            uint64_t p_minus_one = p - 1;
            while (pk <= p_minus_one) {
                uint64_t numerator = p + pk - 1;
                if ((numerator % pk) == 0) {
                    uint64_t y = numerator / pk;
                    if (is_prime(y)) {
                        ok = true;
                        break;
                    }
                }
                if (pk > (uint64_t)-1 / 2) break;
                pk <<= 1;
            }
            if (ok) {
                ++found;
                if (found == n)
                    return (seq_t)p;
            }
        }
        cur_limit *= 2;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
