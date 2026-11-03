/*
 * A256673  Odd numbers with prime arithmetic derivative A003415.
 * https://oeis.org/A256673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd number whose arithmetic derivative is prime
 * Range:     u8 n <= 4, u16 n <= 1343, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 10000 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* available utilities */
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#define CACHE_MAX 20000   /* safely covers n up to ~10000 */

static seq_t  cache[CACHE_MAX];   /* memoised sequence values   */
static idx_t  cache_len = 0;      /* number of cached items */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below the offset is irrelevant */
        return 0;

    /* generate terms until cache contains at least n values */
    while (cache_len < n) {
        /* start from 3 if cache empty, otherwise from next odd after last cached */
        uint64_t cand = (cache_len == 0) ? 3ULL : (uint64_t)cache[cache_len - 1] + 2ULL;

        /* search for the next odd with prime arithmetic derivative */
        while (true) {
            /* factorise cand */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t   cnt;
            factor_u64(cand, primes, exps, &cnt);

            /* need at least 3 distinct prime factors */
            if (cnt < 3) {
                cand += 2ULL;
                continue;
            }

            /* compute arithmetic derivative D = Σ (n/p_i) * e_i */
            seq_t D = 0;
            seq_t n_seq = (seq_t)cand;
            for (size_t i = 0; i < cnt; ++i) {
                seq_t term = n_seq / (seq_t)primes[i];
                term *= (seq_t)exps[i];
                D += term;
            }

            /* check if D is prime */
            if (is_prime_big(D)) {
                if (cache_len < CACHE_MAX)
                    cache[cache_len++] = n_seq;
                else
                    /* should not happen for n <= 10000 */
                    ;
                break;   /* found the next term */
            }

            cand += 2ULL;   /* test next odd candidate */
        }
    }
    return cache[n - 1];
}
