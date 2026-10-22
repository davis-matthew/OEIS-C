/*
 * A392347  Numbers k such that A000005(k) = A000005(k/d + d) for some d.
 * https://oeis.org/A392347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Search implementation of OEIS A392347.
 * A(n) = the nth integer k (k ≥ 2) such that for some divisor d of k
 *        τ(k) = τ(k/d + d), where τ(m) is A000005(m), the number of
 *        divisors of m.
 * The function generates the sequence by sieving divisor counts up to
 * the current maximum and testing each integer for the required
 * property.  It maintains static state so that calls with arbitrary
 * order of n are handled efficiently.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

/* External integer types provided by the build */
extern idx_t _floor_isqrt_u64(idx_t n);

/* ---------- Static state for incremental generation ---------- */
static seq_t *tau = NULL;          /* divisor counts up to current_limit   */
static idx_t  current_limit = 0;   /* largest n for which tau[1..n+1] exist */

static uint64_t *primes = NULL;    /* primes up to sqrt(current_limit+1)   */
static size_t   prime_cnt = 0;     /* number of primes in the array       */

static seq_t *terms      = NULL;   /* collected sequence terms            */
static size_t terms_cap   = 0;      /* allocated capacity for terms        */
static idx_t  terms_count = 0;      /* number of terms currently stored    */

static idx_t next_k_to_check = 1;   /* next integer to evaluate for the
                                        property                              */

/* ---------- Helper: ensure divisor counts & primes ---------------- */
static void ensure_limit(idx_t up_to)
{
    if (up_to <= current_limit) return;

    /* Allocate new array of divisor counts: indices 0..up_to+1 */
    seq_t *new_tau = (seq_t*)malloc((up_to + 2) * sizeof(seq_t));
    if (!new_tau) return;  /* out of memory – abort silently for this task */

    /* Initialise to zero */
    idx_t i;
    for (i = 0; i <= up_to + 1; ++i)
        new_tau[i] = 0;

    /* Sieve for divisor counts */
    for (i = 1; i <= up_to + 1; ++i) {
        for (idx_t j = i; j <= up_to + 1; j += i)
            new_tau[j] += 1;
    }

    /* Replace old array (freeing the old one if present) */
    free(tau);
    tau = new_tau;
    current_limit = up_to;

    /* Rebuild primes up to sqrt(current_limit+1) + 1 */
    free(primes);
    idx_t maxp = _floor_isqrt_u64(current_limit + 1) + 1;
    build_primes_upto(maxp, &primes, &prime_cnt);
}

/* ---------- Helper: append to terms array ----------------------- */
static void append_term(seq_t val)
{
    if (terms_count == terms_cap) {
        size_t newcap = terms_cap ? terms_cap * 2 : 16;
        seq_t *newarr = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
        if (!newarr) return;  /* out of memory – abort silently */
        terms = newarr;
        terms_cap = newcap;
    }
    terms[terms_count++] = val;
}

/* ---------- Helper: generate all divisors of n ------------------ */
static void generate_divisors(idx_t n, uint64_t *divs, size_t *cnt)
{
    size_t dcnt = 1;
    divs[0] = 1;

    idx_t tmp = n;
    for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
        if (tmp % primes[pi] == 0) {
            uint64_t p = primes[pi];
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            size_t old = dcnt;
            uint64_t pow = 1;
            for (int e = 1; e <= exp; ++e) {
                pow *= p;
                for (size_t j = 0; j < old; ++j) {
                    divs[dcnt++] = divs[j] * pow;
                }
            }
        }
    }
    if (tmp > 1) {  /* remaining prime factor */
        uint64_t p = tmp;
        size_t old = dcnt;
        for (size_t j = 0; j < old; ++j) {
            divs[dcnt++] = divs[j] * p;
        }
    }
    *cnt = dcnt;
}

/* ---------- Main sequence function ------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n = 0, but guard anyway */

    while (terms_count < n) {
        /* Ensure we have divisor counts for the next integer to check */
        if (next_k_to_check > current_limit) {
            idx_t new_limit = current_limit * 2 + 10;
            if (new_limit < next_k_to_check)
                new_limit = next_k_to_check;  /* safety for very large k */
            ensure_limit(new_limit);
        }

        idx_t k = next_k_to_check++;
        if (k == 1) continue;  /* 1 is not a term by definition */

        seq_t tau_k = tau[k];

        /* Generate all divisors of k */
        uint64_t divisors[256];
        size_t dcnt;
        generate_divisors(k, divisors, &dcnt);

        bool found = false;
        for (size_t i = 0; i < dcnt; ++i) {
            idx_t d = divisors[i];
            idx_t m = k / d + d;          /* m ≤ k + 1 ≤ current_limit + 1 */
            if (tau[m] == tau_k) {
                found = true;
                break;
            }
        }

        if (found) {
            append_term((seq_t)k);
        }
    }

    return terms[n - 1];
}
