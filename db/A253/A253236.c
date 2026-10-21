/*
 * A253236  The unique prime p <= n such that n-th cyclotomic polynomial has a
 *          root mod p, or 0 if no such p exists.
 * https://oeis.org/A253236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the unique prime p <= n such that the n-th cyclotomic
 *            polynomial has a root mod p; it occurs iff p divides n and the
 *            cofactor m = n / p^k (k = max exponent of p in n) satisfies m |
 *            (p‑1). If no such p exists, the function returns 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *global_primes = NULL;
static size_t global_primes_cnt = 0;
static idx_t global_primes_limit = 0;
static bool primes_built = false;

static void ensure_primes(idx_t n) {
    if (!primes_built || n > global_primes_limit) {
        free(global_primes);
        build_primes_upto(n, &global_primes, &global_primes_cnt);
        global_primes_limit = n;
        primes_built = true;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;  /* n=1 has no primes */

    /* Build or extend prime table up to n */
    ensure_primes(n);

    uint64_t nn = n;
    uint64_t answer = 0;

    /* collect distinct prime factors of n */
    uint64_t factors[64];
    size_t fac_cnt = 0;
    uint64_t temp = nn;
    for (size_t i = 0; i < global_primes_cnt; ++i) {
        uint64_t p = global_primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            factors[fac_cnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {          /* remaining large prime factor */
        factors[fac_cnt++] = temp;
    }

    /* test each prime factor */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = factors[i];
        uint64_t m = nn;
        while (m % p == 0) m /= p;     /* m = n / p^k */
        if ((p - 1) % m == 0) {        /* p-1 divisible by m */
            answer = p;
            break;                    /* uniqueness ensures single solution */
        }
    }

    return (seq_t)answer;
}
