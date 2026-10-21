/*
 * A263729  Primes p where (p - 1)/k - 1 is not prime for any integer k >= 1.
 * https://oeis.org/A263729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 950, u32 n <= 4489, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A263729
   A263729: Primes p such that for every divisor d of p‑1, d‑1 is not prime.
   The function tests successive primes and keeps those whose
   p-1 has no divisor whose subtract‑1 is prime.  Memoisation is used so
   any calling order works. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */
#include "../utils/is_prime_big.h"        /* is_prime_big() */

/* ----- static data ----------------------------------------------------- */
static idx_t *prime_list = NULL;     /* list of primes          */
static size_t prime_cnt  = 0;        /* number of primes returned */
static size_t prime_pos  = 0;        /* next prime to examine   */

static seq_t *seq_cache = NULL;      /* sequence cache          */
static size_t seq_cap   = 0;         /* capacity of cache       */
static size_t seq_cnt   = 0;         /* number of terms cached  */

/* ----- prime list initialisation -------------------------------------- */
static void init_primes(void)
{
    if (prime_list != NULL) return;          /* already initialised */
    /* limit large enough to reach 10 000 terms (heuristic) */
    idx_t limit = 20000000;                  /* 20 million */
    build_primes_upto(limit, (idx_t **)&prime_list, &prime_cnt);
}

/* ----- recursive test of all divisors of m = p-1 ----------------------- */
static bool has_bad_divisor(size_t idx,
                            uint64_t cur,
                            uint64_t *factors,
                            uint64_t *exps,
                            size_t fcnt)
{
    if (idx == fcnt) {                       /* reached one divisor */
        if (cur > 1) {                       /* ignore divisor 1    */
            seq_t cand = (seq_t)(cur - 1);
            if (is_prime_big(cand))
                return true;                /* divisor gives prime */
        }
        return false;
    }

    /* try all exponents for prime factors[idx] */
    uint64_t prod = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        if (has_bad_divisor(idx + 1, cur * prod, factors, exps, fcnt))
            return true;
        prod *= factors[idx];
    }
    return false;
}

/* ----- sequence generation ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the offset 1 */

    init_primes();

    /* grow cache if needed */
    if (seq_cap < (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (seq_cnt < (size_t)n) {
        if (prime_pos >= prime_cnt) {
            /* not enough primes precomputed – unlikely given chosen limit */
            break;
        }
        uint64_t p = prime_list[prime_pos++];

        uint64_t m = p - 1;
        uint64_t tmp = m;

        /* factor m: store distinct primes and their exponents */
        const size_t MAX_FACTORS = 64;
        uint64_t factors[MAX_FACTORS];
        uint64_t exps[MAX_FACTORS];
        size_t fcnt = 0;

        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
            uint64_t q = prime_list[i];
            if (tmp % q == 0) {
                uint64_t e = 0;
                while (tmp % q == 0) {
                    tmp /= q;
                    ++e;
                }
                factors[fcnt] = q;
                exps[fcnt]    = e;
                ++fcnt;
            }
        }
        if (tmp > 1) {
            factors[fcnt] = tmp;
            exps[fcnt] = 1;
            ++fcnt;
        }

        if (!has_bad_divisor(0, 1, factors, exps, fcnt)) {
            seq_cache[seq_cnt++] = (seq_t)p;
        }
    }

    return seq_cache[n - 1];
}
