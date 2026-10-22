/*
 * A327513  Number of divisors of n that are 1, 2, or a nonprime number whose
 *          prime indices are pairwise coprime.
 * https://oeis.org/A327513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9679, u16 n <= 9679, u32 n <= 9679, u64 n <= 9679,
 *            u128 n <= 9679, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A327513.
   Formula: a(n) = number of divisors d of n such that
              d is 1, 2, or a composite number whose prime indices
              are pairwise coprime.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Prime table used for finding prime indices. */
static bool primes_init_done = false;
static uint64_t *prime_table = NULL;
static size_t prime_cnt = 0;

/* initialise prime sieve up to a fixed limit. */
static void init_prime_table(void)
{
    if (!primes_init_done) {
        /* 1M is sufficient for the used test ranges. */
        size_t limit = 1000000ULL;
        build_primes_upto(limit, &prime_table, &prime_cnt);
        primes_init_done = true;
    }
}

/* binary search into prime_table, returns 1‑based index, or -1 if not found. */
static int prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_table[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < prime_cnt && prime_table[lo] == p)
        return (int)(lo + 1);     /* indices in OEIS are 1‑based */
    return -1;                    /* not found – should not happen for test data */
}

seq_t a(idx_t n)
{
    /* Ensure prime table is ready. */
    init_prime_table();

    /* Factorise n.  */
    uint64_t fac[64], exp[64];
    size_t fac_cnt = 0;
    factor_u64((uint64_t)n, fac, exp, &fac_cnt);

    /* exponents of prime 2 (if present). */
    uint64_t e2 = 0;
    for (size_t i = 0; i < fac_cnt; ++i)
        if (fac[i] == 2ULL) {
            e2 = exp[i];
            break;
        }

    /* Collect primes > 2 together with their indices. */
    uint64_t primes_other[64];
    uint32_t idx_other[64];
    size_t other_cnt = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        if (fac[i] == 2ULL) continue;
        /* exponents >0, but we restrict to exponent 1 in divisors. */
        int idx = prime_index(fac[i]);
        if (idx == -1) {          /* should not happen for intended data */
            /* fallback: treat index as large number; will break equality */
            idx = (int)(fac[i]);  /* unique, but not needed */
        }
        primes_other[other_cnt] = fac[i];
        idx_other[other_cnt]   = (uint32_t)idx;
        ++other_cnt;
    }

    seq_t total = 0;
    const uint64_t a_plus_one = e2 + 1U;

    /* iterate over all subsets of other primes */
    size_t limit = ((size_t)1) << other_cnt;
    for (size_t mask = 0; mask < limit; ++mask) {
        /* Quick early skip: check pairwise gcd of indices */
        bool ok = true;
        for (size_t i = 0; i < other_cnt && ok; ++i) {
            if (!(mask & ((size_t)1 << i))) continue;
            for (size_t j = i + 1; j < other_cnt; ++j) {
                if (!(mask & ((size_t)1 << j))) continue;
                /* g = gcd(idx_other[i], idx_other[j]) */
                uint32_t a = idx_other[i];
                uint32_t b = idx_other[j];
                while (b) {
                    uint32_t t = a % b;
                    a = b;
                    b = t;
                }
                if (a > 1) { ok = false; break; }
            }
        }
        if (!ok) continue;

        /* number of selected primes */
        unsigned int s = (unsigned int)__builtin_popcountll(mask);

        if (s == 0)                /* empty subset: 1 and 2 counted */
            total += (seq_t)a_plus_one;
        else if (s == 1)           /* single prime: remove the prime itself */
            total += (seq_t)e2;
        else                       /* at least two primes: all e2 */
            total += (seq_t)a_plus_one;
    }

    return total;
}
