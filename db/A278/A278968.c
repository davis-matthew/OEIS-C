/*
 * A278968  Least number k such that pk is of minimal Hamming weight, where p
 *          is the n-th prime.
 * https://oeis.org/A278968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, seq_cmp, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A278968.  
   For the n‑th prime p, find the least k such that p*k has the minimum
   possible Hamming weight (number of 1‑bits).  The minimum weight is 1 for
   p = 2; for odd primes it is 2 when the order of 2 mod p is even, and
   3 otherwise.  The function below searches for the smallest k achieving
   that weight, using a sieve for primes, factorisation of p‑1, and
   mapping of powers of two modulo p.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/seq_pow.h"
#include "../utils/seq_cmp.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* Small‑integer modular exponentiation used only for computing the
   multiplicative order of 2 mod p.  128‑bit intermediate ensures
   correctness up to 64‑bit modulus. */
static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) {
            result = (unsigned __int128)result * base % mod;
        }
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* On‑demand sieve for the first N primes.  We grow the sieve as
   necessary; the array `prime_list` is kept static for reuse between
   calls. */
static bool primes_prepared = false;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static uint64_t prime_limit = 0;

static void ensure_primes(idx_t n) {
    if (prime_count >= (size_t)n)
        return;
    if (!primes_prepared) {
        prime_limit = 30;          /* start with a small limit */
    } else {
        prime_limit *= 2;          /* double until we have enough */
    }
    while (true) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        build_primes_upto(prime_limit, &prime_list, &prime_count);
        if (prime_count >= (size_t)n)
            break;
        prime_limit *= 2;
    }
    primes_prepared = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1 */

    /* get n‑th prime */
    ensure_primes(n);
    uint64_t p = prime_list[(size_t)(n - 1)];

    /* special case p = 2 */
    if (p == 2)
        return (seq_t)1;

    /* ------------------------------------------------------------------
       Compute multiplicative order of 2 modulo p.  This requires
       factoring p‑1.  */
    uint64_t phi = p - 1;

    /* factor phi */
    uint64_t prim[64], expo[64];
    size_t pc = 0;
    factor_u64(phi, prim, expo, &pc);   /* returns unique prime factors */

    uint64_t order = phi;
    for (size_t i = 0; i < pc; ++i) {
        uint64_t q = prim[i];
        if (q == 0) continue;
        while (order % q == 0 && pow_mod64(2, order / q, p) == 1) {
            order /= q;
        }
    }

    /* ------------------------------------------------------------------
       If order is even, minimal weight is 2.  Find smallest k. */
    if ((order & 1ULL) == 0) {
        seq_t pow2 = seq_pow((seq_t)2, (idx_t)(order / 2));
        seq_t k = (pow2 + (seq_t)1) / (seq_t)p;
        return k;
    }

    /* ------------------------------------------------------------------
       Order is odd → minimal weight is 3.  We look for the smallest
       i > j such that 2^i + 2^j + 1 is a multiple of p.  The strategy
       is to precompute a table mapping each residue 2^k (mod p) to k
       (0 ≤ k < order). */
    uint32_t *exp_by_rem = (uint32_t *)calloc(p, sizeof(uint32_t));
    if (!exp_by_rem)
        return (seq_t)0;   /* memory failure, shouldn't happen */

    const uint32_t NONE = 0xFFFFFFFFu;
    for (size_t i = 0; i < p; ++i) exp_by_rem[i] = NONE;

    uint64_t rem = 1 % p;
    exp_by_rem[rem] = 0;
    for (uint64_t i = 1; i < order; ++i) {
        rem = (rem * 2) % p;
        exp_by_rem[rem] = (uint32_t)i;
    }

    /* iterate over j, using rolling modulus to avoid recomputation */
    bool found = false;
    seq_t best_k = 0;
    uint64_t rem_j = 1 % p;
    for (uint64_t j = 0; j < order; ++j) {
        if (j > 0)
            rem_j = (rem_j * 2) % p;

        /* need ≡ -(2^j + 1) (mod p) */
        uint64_t need = (p - ((rem_j + 1) % p)) % p;
        uint32_t exp = exp_by_rem[need];
        if (exp != NONE && (uint64_t)exp > j) {
            seq_t pow_i = seq_pow((seq_t)2, (idx_t)exp);
            seq_t pow_j = seq_pow((seq_t)2, (idx_t)j);
            seq_t numerator = pow_i + pow_j + (seq_t)1;
            seq_t k = numerator / (seq_t)p;
            if (!found || seq_cmp(&k, &best_k) < 0) {
                best_k = k;
                found = true;
            }
        }
    }

    free(exp_by_rem);
    /* Theoretically, a solution should always exist for odd p. */
    return found ? best_k : (seq_t)1;
}
