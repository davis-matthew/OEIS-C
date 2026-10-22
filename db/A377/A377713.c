/*
 * A377713  Squarefree composite k such that floor(log n/log lpf(k)) <=
 *          omega(k), where lpf = A020639 and omega = A001221.
 * https://oeis.org/A377713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4467, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A377713
 *
 * a(n) = the nth square‑free composite integer k such that
 *        floor(log(k) / log(lpf(k))) <= omega(k),
 * where lpf(k) is the lowest prime factor and omega(k) is the number
 * of distinct prime factors.  The sequence is 6, 15, 21, 35, …
 *
 * The implementation generates eligible k in ascending order,
 * caching the results for fast random access.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

/* ------------------------ cache ------------------------ */
static seq_t *cache = NULL;   /* stored sequence values */
static size_t cache_cap = 0;   /* capacity in elements */
static size_t cache_used = 0;  /* number of elements stored */

/* next integer to test (>=6) */
static uint64_t next_candidate = 5;  /* first increment will make it 6 */

/* temporary arrays for factor_u64 (max 64 prime factors) */
static uint64_t *t_primes = NULL;
static uint64_t *t_exps   = NULL;
#define T_MAX_PR 64

static void init_temp_arrays(void)
{
    if (!t_primes) {
        t_primes = (uint64_t *)malloc(T_MAX_PR * sizeof(uint64_t));
        t_exps   = (uint64_t *)malloc(T_MAX_PR * sizeof(uint64_t));
    }
}

/*
 * Determine whether n is square‑free composite.
 * If so, return its number of distinct prime factors in *omega_out
 * and its lowest prime factor in *lpf_out.
 *
 * Return true iff n is square‑free composite.
 */
static bool squarefree_composite_info(uint64_t n,
                                      size_t *omega_out,
                                      uint64_t *lpf_out)
{
    size_t cnt = 0;
    init_temp_arrays();
    factor_u64(n, t_primes, t_exps, &cnt);

    if (cnt < 2)          /* prime or prime‑power */
        return false;

    /* all exponents must be 1 for square‑free */
    for (size_t i = 0; i < cnt; ++i)
        if (t_exps[i] != 1)
            return false;

    *omega_out = cnt;

    /* smallest prime factor */
    uint64_t minp = t_primes[0];
    for (size_t i = 1; i < cnt; ++i)
        if (t_primes[i] < minp)
            minp = t_primes[i];
    *lpf_out = minp;
    return true;
}

/* --------------------- sequence function --------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n==0, return 0 */
        return 0;

    /* generate values until at least n terms are cached */
    while (cache_used < (size_t)n) {
        /* advance to next integer to test */
        do { ++next_candidate; } while (next_candidate < 6);

        if (is_prime(next_candidate))
            continue;  /* skip primes */

        size_t omega;
        uint64_t lpf;
        if (!squarefree_composite_info(next_candidate, &omega, &lpf))
            continue;  /* not square‑free composite */

        /* check k < lpf^(omega+1) without overflow
           by multiplying stepwise and aborting early.        */
        seq_t candidate = (seq_t)next_candidate;
        seq_t power = (seq_t)1;
        bool ok = false;

        for (size_t i = 0; i < omega + 1; ++i) {
            /* if power > candidate / lpf, the next multiply
               would exceed candidate → condition satisfied.   */
            seq_t lim = candidate / lpf;
            if (power > lim) {
                ok = true;
                break;
            }
            power = power * (seq_t)lpf;
        }

        /* after loop, if power > candidate, we also satisfy */
        if (!ok && candidate < power)
            ok = true;

        if (!ok)
            continue;  /* candidate fails inequality */

        /* ensure cache capacity */
        if (cache_used == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_used++] = candidate;   /* store term */
    }

    return cache[n - 1];
}
