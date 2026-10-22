/*
 * A334212  Least number k such that n^k + 1 is not squarefree.
 * https://oeis.org/A334212
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   build_primes, mul_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A334212: Least k such that n^k + 1 is not squarefree.
 *  Computation:  for each prime p∤n (p≤n+1) we test whether
 *  n^k ≡ -1 (mod p²).  The smallest such k is returned.
 *  Special case: if n ≡ 3 (mod 4) then 4 | (n + 1), so k = 1.
 *
 *  The algorithm uses a pre‑generated list of primes up to 10001
 *  (the maximum n+1 for the range of interest).  The multiplicative
 *  order of n modulo p² is found by factoring φ(p²)=p(p‑1) using
 *  that prime list.  All arithmetic fits in 64 bits and uses
 *  unsigned 128‑bit multiplication when necessary.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/mul_mod_u64.h"

/* --- modular multiplication using 128‑bit arithmetic -------------------------------- */

/* --- modular exponentiation (base^exp mod mod) -------------------------------------- */
static inline uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1ULL)
            res = mul_mod_u64(res, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

/* --- global prime table ----------------------------------------------------------- */
static uint64_t *prime_tbl = NULL;   /* uint64_t array of primes */
static size_t     prime_cnt = 0;
static idx_t      prime_limit = 0;

/* Build the prime table up to at least 'lim' (inclusive).  Uses build_primes(). */
static void ensure_prime_table(idx_t lim)
{
    if (prime_tbl && prime_limit >= lim)
        return;
    if (prime_tbl) {
        free(prime_tbl);
        prime_tbl = NULL;
    }
    build_primes(lim, (idx_t**)&prime_tbl, &prime_cnt);
    prime_limit = lim;
}

/* --- compute multiplicative order of n modulo p^2 --------------------------------- */
static idx_t order_mod_p2(uint64_t p, uint64_t n)
{
    uint64_t mod  = p * p;            /* p^2, fits in 64 bits for p ≤ 10001 */
    uint64_t base = n % mod;

    /* φ(p²) = p(p-1) */
    uint64_t phi  = p * (p - 1);
    uint64_t order = phi;

    /* factor p-1 using the primes list */
    uint64_t t = p - 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_tbl[i];
        if (q * q > t)
            break;
        if (t % q != 0ULL)
            continue;
        while (t % q == 0ULL)
            t /= q;
        while (order % q == 0ULL) {
            uint64_t cand = order / q;
            if (pow_mod_u64(base, cand, mod) == 1ULL)
                order = cand;
            else
                break;
        }
    }
    if (t > 1ULL) {                    /* remaining prime factor */
        uint64_t q = t;
        while (order % q == 0ULL) {
            uint64_t cand = order / q;
            if (pow_mod_u64(base, cand, mod) == 1ULL)
                order = cand;
            else
                break;
        }
    }
    /* factor p itself */
    while (order % p == 0ULL) {
        uint64_t cand = order / p;
        if (pow_mod_u64(base, cand, mod) == 1ULL)
            order = cand;
        else
            break;
    }
    return (idx_t)order;
}

/* --- public entry point ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2ULL)
        return 0;                     /* values below offset are unspecified */

    /* n ≡ 3 (mod 4) gives k = 1, since 4 | (n + 1) */
    if ((n & 3ULL) == 3ULL)
        return 1;

    /* primes up to n+1 are enough to find the minimal k (OEIS bounds). */
    idx_t max_prime = n + 1ULL;
    ensure_prime_table(max_prime);

    idx_t best_k = 0;                 /* 0 = not set */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p > max_prime)           /* stop once we exceed n+1 */
            break;
        if (p == 2ULL)               /* p=2 never gives a square factor for odd n */
            continue;
        if (n % p == 0ULL)
            continue;                /* p divides n */

        uint64_t mod = p * p;
        uint64_t base = n % mod;

        /* k = 1 check: p^2 | n+1 */
        if (base == mod - 1ULL) {    /* n ≡ -1 (mod p²) */
            return 1;
        }

        /* compute multiplicative order of n mod p² */
        idx_t ord = order_mod_p2(p, n);

        if ((ord & 1ULL) != 0ULL)    /* odd order => cannot yield -1 */
            continue;

        idx_t k_candidate = ord >> 1;   /* k = order / 2 */

        if (pow_mod_u64(base, k_candidate, mod) == mod - 1ULL) {
            if (best_k == 0 || k_candidate < best_k) {
                best_k = k_candidate;
                if (best_k == 1)        /* cannot beat k=1 */
                    return best_k;
            }
        }
    }

    /* The OEIS guarantees a solution, so best_k should be set. */
    return (seq_t)best_k;
}
