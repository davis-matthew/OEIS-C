/*
 * A283268  Smallest b-a such that a < prime(n)^2 < b, where a,b are
 *          semiprimes.
 * https://oeis.org/A283268
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest b-a such that a < prime(n)^2 < b, where a,b are
 *            semiprimes (product of exactly two primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes_list = NULL;
static size_t       primes_cnt  = 0;
static bool        primes_init  = false;

static void init_primes(void) {
    if (primes_init) return;
    const idx_t LIMIT = 200000;                  /* > sqrt(prime(10000)^2) */
    build_primes_upto(LIMIT, &primes_list, &primes_cnt);
    primes_init = true;
}

/* Returns true iff n is semiprime (product of exactly two primes,
   counting multiplicity). */
static bool is_semiprime_u64(uint64_t n, const uint64_t *primes, size_t cnt) {
    if (n < 4) return false;                    /* the smallest semiprime is 4 = 2*2 */
    int factors = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            ++factors;
            temp /= p;
            while (temp % p == 0) {
                ++factors;
                temp /= p;
                if (factors > 2) return false;    /* more than two prime factors */
            }
            if (factors > 2) return false;
        }
    }
    if (temp > 1) ++factors;                     /* remaining prime factor */
    return factors == 2;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;                         /* undefined for n < 2 */
    init_primes();

    uint64_t p = get_prime(n);                   /* n-th prime */
    uint64_t s = p * p;                          /* square of the prime */

    /* find semiprime < s */
    uint64_t down = s - 1;
    while (down >= 4 &&
           !is_semiprime_u64(down, primes_list, primes_cnt))
        --down;

    /* find semiprime > s */
    uint64_t up = s + 1;
    while (!is_semiprime_u64(up, primes_list, primes_cnt))
        ++up;

    return (seq_t)(up - down);
}
