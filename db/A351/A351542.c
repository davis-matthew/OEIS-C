/*
 * A351542  Even numbers k that have an odd prime factor p such that p also
 *          divides sigma(k), but valuation(k,p) differs from
 *          valuation(sigma(k), p), and p does not divide A003961(k), where
 *          A003961 is fully multiplicative with a(p) = nextprime(p), and
 *          sigma is the sum of divisors function.
 * https://oeis.org/A351542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   factor_u64, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A351542: even numbers k that have an odd prime factor p such that
 *  p divides σ(k), but the exponent of p in k differs from its exponent in
 *  σ(k), and p is not a next‐prime of any prime divisor of k
 *  (equivalently, the previous odd prime does not divide k).
 *  This implementation searches for the next qualifying k for each request
 *  and memoises results.  The algorithm is O(n·(log k)) and fast for
 *  n ≤ 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache = NULL;          /* memoised values */
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static uint64_t last_checked = 1;     /* largest k that has been examined */

/* -------------------------------------------------------------------------- */
/*  is_special(k)  -- returns true if k satisfies the definition of A351542  */
static bool is_special(uint64_t k)
{
    /* an admissible k must be even */
    if ((k & 1) != 0) return false;

    /* factorise k (exposing both primes and exponents) */
    uint64_t pf_pr[64], pf_ex[64];
    size_t pf_cnt = 0;
    factor_u64(k, pf_pr, pf_ex, &pf_cnt);

    /* pre‑compute σ(k) once */
    seq_t sigma_val = sigma_of((idx_t)k);

    /* examine each odd prime factor of k */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf_pr[i];
        if ((p & 1) == 0) continue;            /* skip the factor 2 */

        /* requirement: the previous odd prime of p must not divide k */
        idx_t prev = prev_prime(p);            /* previous prime of p */
        if (prev != 0 && (k % (uint64_t)prev) == 0) {
            continue;
        }

        /* p must divide σ(k) */
        if ((sigma_val % (seq_t)p) != 0) {
            continue;
        }

        /* compare the exponents of p in k and in σ(k) */
        int exp_k = (int)pf_ex[i];
        int exp_sigma = 0;
        seq_t tmp = sigma_val;
        while ((tmp % (seq_t)p) == 0) {
            tmp /= (seq_t)p;
            ++exp_sigma;
        }

        if (exp_k != exp_sigma) {
            return true;   /* all conditions satisfied */
        }
    }
    return false;   /* no qualifying odd prime factor found */
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is undefined */

    /* expand cache until it contains at least n elements */
    while (cache_size < (size_t)n) {
        uint64_t k = last_checked + 1;
        while (true) {
            if (is_special(k)) {
                append_candidate(&cache, &cache_size, &cache_cap, (seq_t)k);
                if (cache_size == (size_t)n) break;
            }
            ++k;
        }
        /* k is the last number examined; record it for next request */
        last_checked = k;
    }
    return cache[n - 1];
}
