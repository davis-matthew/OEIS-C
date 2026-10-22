/*
 * A380540  a(n) is the multiplicative order of -7 modulo prime(n); a(4) = 0
 *          for completion.
 * https://oeis.org/A380540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multiplicative order of (-7) modulo prime(n) (a(4) = 0
 *            because the order is undefined modulo 7)
 * Range:     u8 n <= 34, u16 n <= 4602, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"

static uint64_t *small_primes = NULL;
static size_t small_prime_cnt = 0;

static void init_small_primes(void) {
    if (small_primes) return;
    /* 400 is > sqrt(104729) so all needed trial factors are covered. */
    build_primes_upto(400, &small_primes, &small_prime_cnt);
}

seq_t a(idx_t n) {
    init_small_primes();

    uint32_t p = get_prime(n);            /* nth prime, 1‑based, as uint32_t */
    if (p == 7)                           /* special case: order undefined */
        return (seq_t)0;

    /* (-7) mod p  ==  (2p-7) mod p  ==  p-7  (for p>7)  */
    seq_t base = (seq_t)((uint64_t)p * 2 - 7) % (seq_t)p;

    uint64_t ord = (uint64_t)p - 1;       /* φ(p)  */
    uint64_t temp = ord;
    uint64_t factors[32];
    size_t factor_cnt = 0;

    /* factor ord using the small primes table */
    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint64_t q = small_primes[i];
        if (q * q > temp) break;
        if (temp % q == 0) {
            while (temp % q == 0)
                temp /= q;
            factors[factor_cnt++] = q;
        }
    }
    if (temp > 1)
        factors[factor_cnt++] = temp;      /* remaining prime factor */

    /* reduce the order by removing prime factors whenever possible   */
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t q = factors[i];
        if (ord % q == 0) {
            uint64_t candidate;
            while (ord % q == 0) {
                candidate = ord / q;
                if (mod_pow(base, candidate, p) != (seq_t)1)
                    break;
                ord = candidate;
            }
        }
    }

    return (seq_t)ord;
}
