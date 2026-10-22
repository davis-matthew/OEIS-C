/*
 * A322359  Least common multiple of product (p-1) and product (p+1), where p
 *          ranges over distinct prime divisors of n.
 * https://oeis.org/A322359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(∏(p-1), ∏(p+1)) over distinct prime divisors p of n
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   build_primes_upto, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_gcd.h"

static uint64_t *prime_list = NULL;   /* primes up to the current limit */
static size_t   prime_cnt  = 0;       /* number of primes in the list */
static idx_t    prime_limit = 0;      /* highest n for which primes were built */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined below offset */

    /* n == 1 has no prime factors → product = 1 */
    if (n == 1) return (seq_t)1;

    /* Ensure we have all primes up to n (needed for factorisation) */
    if (n > prime_limit) {
        uint64_t *tmp = NULL;
        size_t   tmp_cnt = 0;
        build_primes_upto((idx_t)n, &tmp, &tmp_cnt);
        free(prime_list);
        prime_list = tmp;
        prime_cnt  = tmp_cnt;
        prime_limit = n;
    }

    uint64_t temp = (uint64_t)n;
    seq_t prod_minus = 1;      /* ∏ (p-1) */
    seq_t prod_plus  = 1;      /* ∏ (p+1) */

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            /* remove all powers of p */
            do { temp /= p; } while (temp % p == 0);
            prod_minus = prod_minus * ((seq_t)p - 1);
            prod_plus  = prod_plus  * ((seq_t)p + 1);
        }
    }
    /* Remaining factor may be a prime > sqrt(original n) */
    if (temp > 1) {
        uint64_t p = temp;
        prod_minus = prod_minus * ((seq_t)p - 1);
        prod_plus  = prod_plus  * ((seq_t)p + 1);
    }

    seq_t g = seq_gcd(prod_minus, prod_plus);
    return (prod_minus / g) * prod_plus;   /* lcm = a/g * b */
}
