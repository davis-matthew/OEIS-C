/*
 * A284943  Expansion of Sum_{p prime, k>=1} x^(p^k)*(1 - x)^2/(1 - 2*x)^2.
 * https://oeis.org/A284943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime, k>=1, p^k <= n} * occ(n, m) where m = p^k.
 *            * occ(n, m) = 1 if m == n * = 2 if m == n-1 * = 2^(n-m-2) *
 *            (n-m+3) otherwise * * This follows from counting, for each part
 *            size m, * how many times m appears in all compositions of n
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

static uint64_t *sieve_primes = NULL;
static size_t sieve_prime_cnt = 0;
static idx_t  sieve_limit = 0;

/* compute the number of prime-power parts in all compositions of n */
seq_t a(idx_t n)
{
    /* extend the sieve if needed */
    if (n > sieve_limit) {
        free(sieve_primes);
        build_primes_upto(n, &sieve_primes, &sieve_prime_cnt);
        sieve_limit = n;
    }

    seq_t result = 0;

    for (size_t i = 0; i < sieve_prime_cnt; ++i) {
        uint64_t p = sieve_primes[i];

        /* iterate over all powers of p ≤ n */
        uint64_t power = p;
        while (power <= n) {
            idx_t k = n - power;     /* remaining sum after using this part */

            seq_t occ;
            if (k == 0) {
                occ = 1;
            } else if (k == 1) {
                occ = 2;
            } else {
                /* 2^(k-2) * (k+3) */
                occ = pow2((idx_t)(k - 2)) * ((seq_t)(k + 3));
            }

            result += occ;

            /* next power of p, guard against overflow */
            if (power > n / p)
                break;
            power *= p;
        }
    }

    return result;
}
