/*
 * A246374  Primes p such that if 2p-1 = product_{k >= 1} A000040(k)^(c_k),
 *          then p > product_{k >= 1} A000040(k-1)^(c_k).
 * https://oeis.org/A246374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1785, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64, is_prime, seq_cmp,
 *            seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A246374: primes p such that if 2p‑1 = ∏ q_i^{e_i}
   then p > ∏ prev_prime(q_i)^{e_i}  (prev_prime(q) = largest prime < q) */

/* required for the helper utilities */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* utilities provided by the build */
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"

/* -------------------------------------------------------------------- */
/* static data: sieve of primes                                 */
/* -------------------------------------------------------------------- */
static uint64_t *primes = NULL;   /* array of primes up to LIMIT */
static size_t   prime_cnt = 0;    /* number of primes stored */
static const idx_t LIMIT = 10000000;  /* should cover 2p‑1 for required n */

/* initialise the prime table once */
static void init_primes(void)
{
    if (primes == NULL)
        build_primes_upto(LIMIT, &primes, &prime_cnt);
}

/* -------------------------------------------------------------------- */
/* helper: previous prime of an odd prime q (q ≥ 3)                  */
/* -------------------------------------------------------------------- */
static uint64_t prev_prime_of(uint64_t q)
{
    if (q <= 3)    /* prev_prime(3) = 2;  q=2 never occurs */
        return 2;

    /* use a downward search; gaps between primes are < 100 on this range */
    uint64_t m = q - 1;
    while (m >= 2 && !is_prime((idx_t)m))
        --m;
    return m;
}

/* -------------------------------------------------------------------- */
/* cache of computed sequence values   (static, memoised)             */
/* -------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate_index = 0;   /* index in primes array */

/* -------------------------------------------------------------------- */
/* main sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise prime table once */
    init_primes();

    /* grow cache if necessary */
    while (cache_len < n) {
        /* ensure we still have primes to test */
        if (next_candidate_index >= prime_cnt)
            return (seq_t)0;   /* should not happen for requested n */

        /* take next prime */
        uint64_t p = primes[next_candidate_index++];
        if (p <= 2) continue;          /* skip p = 2 (2p‑1 is prime) */

        uint64_t N = 2 * p - 1;         /* 2p‑1 is odd */
        if (is_prime((idx_t)N))         /* must be composite */
            continue;

        /* factorise N */
        uint64_t fac_pr[64];
        uint64_t fac_ex[64];
        size_t   fac_cnt = 0;
        factor_u64(N, fac_pr, fac_ex, &fac_cnt);

        /* compute RHS = ∏ prev_prime(q_i)^{e_i} */
        seq_t rhs = 1;
        for (size_t i = 0; i < fac_cnt; ++i) {
            uint64_t q = fac_pr[i];
            uint64_t e = fac_ex[i];
            seq_t  pow_val = big_pow((seq_t)prev_prime_of(q), (seq_t)e);
            rhs = seq_mul(rhs, pow_val);
        }

        seq_t p_seq = (seq_t)p;
        /* check condition p > RHS */
        if (seq_cmp(&p_seq, &rhs) > 0) {
            /* add to cache */
            if (cache_cap <= cache_len) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = p_seq;
        }
    }
    return cache[n - 1];
}
