/*
 * A382224  Numbers k such that every element with maximal order mod k is
 *          prime.
 * https://oeis.org/A382224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   append_candidate, factor_u64, gcd_u64, is_prime_small,
 *            lambda_of_prime_power, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A382224.
   Computes the sequence by brute force: for each k >= 3,
   every element a (1 < a < k) with ord_k(a) = λ(k) must be prime.
   If a composite with maximal order is found, k is rejected. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/append_candidate.h"
#include "../utils/is_prime_small.h"
#include "../utils/powmod64.h"
#include "../utils/gcd_u64.h"
#include "../utils/lambda_of_prime_power.h"

/* Euclid gcd for uint64_t */

/* Carmichael function λ(n) from factorization arrays */
static uint64_t carmichael(uint64_t n) {
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    uint64_t lam = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t lam_p = lambda_of_prime_power(p, e);
        uint64_t g = gcd_u64(lam, lam_p);
        lam = (uint64_t)((__int128)lam / g * lam_p);
    }
    return lam;
}

/* Append value to dynamic seq_t array (uses append_candidate helper). */
static void ensure_terms_capacity(size_t needed, size_t *cap, seq_t **ptr) {
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    *ptr = (seq_t*)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Main function */
seq_t a(idx_t n) {
    /* static state for memoization */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t max_checked = 2;  /* last k processed */

    /* Return cached result if available */
    if ((size_t)n <= terms_len)
        return terms[(size_t)n - 1];

    /* Prepare buffer for factorization */
    uint64_t primes[64], exps[64], lam_primes[64], lam_exps[64];
    size_t kcnt, lamcnt;

    /* Scan candidate k until we have enough terms */
    for (uint64_t k = max_checked + 1; ; ++k) {
        bool ok = true;

        /* Compute Carmichael λ(k) */
        uint64_t lam = carmichael(k);

        /* Factor λ(k) to get its prime divisors and λ/prime */
        factor_u64(lam, lam_primes, lam_exps, &lamcnt);
        uint64_t lam_div[32];
        size_t lam_div_cnt = 0;
        for (size_t i = 0; i < lamcnt; ++i) lam_div[lam_div_cnt++] = lam / lam_primes[i];

        /* Scan all a>1 for maximal order with composite a */
        for (uint64_t a_val = 2; a_val < k; ++a_val) {
            /* Skip non coprime */
            if (gcd_u64(a_val, k) != 1) continue;

            /* Skip primes: only composites matter */
            if (is_prime_small(a_val)) continue;

            /* Test if ord(a_val) == lam */
            bool max_order = true;
            for (size_t i = 0; i < lam_div_cnt; ++i) {
                if (powmod64(a_val, lam_div[i], k) == 1ULL) {
                    max_order = false;
                    break;
                }
            }
            if (max_order) {  /* composite with maximal order found */
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Record term */
            ensure_terms_capacity(terms_len + 1, &terms_cap, &terms);
            terms[terms_len++] = (seq_t)k;
        }

        /* Stop when enough terms found */
        if ((size_t)n <= terms_len) {
            max_checked = k;
            return terms[(size_t)n - 1];
        }
    }
}
