/*
 * A242345  Number of primes p < prime(n) with p and 2^p - p both primitive
 *          roots modulo prime(n).
 * https://oeis.org/A242345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 3500, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   build_primes_upto, factorize, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = number of primes p < prime(n) such that both p and
   2^p - p are primitive roots modulo prime(n).  The implementation
   uses a small sieve for the primes < prime(n) and simple lifting of
   the primitive‑root test. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t n); */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt); */
#include "../utils/factorize.h"          /* size_t factorize(uint64_t n, uint64_t *primes, int *exps); */
#include "../utils/mod_pow.h"            /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* offset is 1 as in OEIS; return 0 for indices < 1 to be safe */
    if (n == 0) return 0;

    /* prime(n) only fits in 32 bits for n ≤ 10000 */
    uint32_t P = get_prime(n);
    if (P <= 2) return 0;   /* no prime < 2 */

    /* gather all primes < P */
    uint64_t *prime_list = NULL;
    size_t prime_cnt   = 0;
    build_primes_upto((idx_t)P - 1, &prime_list, &prime_cnt);

    /* factorise P‑1 to collect its distinct prime divisors */
    uint64_t pf[64];
    int      exp[64];      /* exponents not needed */
    size_t pf_cnt = factorize((uint64_t)P - 1, pf, exp);

    seq_t count = 0;

    /* iterate over primes < P */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];   /* candidate prime */

        /* check that q is a primitive root modulo P */
        bool ok = true;
        for (size_t j = 0; j < pf_cnt && ok; ++j) {
            uint64_t e = ((uint64_t)P - 1) / pf[j];
            if (mod_pow((seq_t)q, e, (seq_t)P) == 1) ok = false;
        }
        if (!ok) continue;

        /* compute (2^q – q) mod P */
        seq_t pow2 = mod_pow((seq_t)2, q, (seq_t)P);
        seq_t t = pow2 + (seq_t)P - (seq_t)q;   /* q < P, safe cast */
        if (t >= (seq_t)P) t -= (seq_t)P;

        if (t == (seq_t)0) continue;   /* 0 cannot be a primitive root */

        /* check that t is a primitive root modulo P */
        for (size_t j = 0; j < pf_cnt && ok; ++j) {
            uint64_t e = ((uint64_t)P - 1) / pf[j];
            if (mod_pow(t, e, (seq_t)P) == 1) ok = false;
        }
        if (ok) ++count;
    }

    free(prime_list);
    return count;
}
