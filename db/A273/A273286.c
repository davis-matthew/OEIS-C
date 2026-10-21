/*
 * A273286  Positive integers n such that n=p+q for some primes p,q with
 *          pi(p)*pi(q) = sigma(n).
 * https://oeis.org/A273286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------------
  Formula: a(n) is the n-th even integer n=p+q, p and q primes, such
  that sigma(n) = pi(p) * pi(q).  The sequence starts 92,130,132,...
  --------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------
   global tables
------------------------------------------------------------------*/
static uint64_t  *primes   = NULL;  /* primes in ascending order          */
static size_t      primes_cnt   = 0; /* number of primes                   */
static idx_t    *pi_arr   = NULL;  /* pi_arr[x] = number of primes <= x  */
static idx_t        maxN   = 0;      /* current maximum n considered       */

static seq_t     *solutions = NULL; /* found sequence terms                */
static size_t      sol_cnt  = 0;    /* number of solutions found           */
static size_t      sol_cap  = 0;    /* current capacity of solutions array */
static idx_t        next_test = 4;  /* next even n to test (>=4)           */

/* ------------------------------------------------------------------
   Extend precomputation tables up to new_lim (inclusive).  Works
   for C and C++ (casts added for realloc/malloc return types).
------------------------------------------------------------------*/
static void extend_limit(idx_t new_lim)
{
    if (new_lim <= maxN)
        return;

    /* Reallocate pi_arr to size new_lim+1 */
    idx_t *new_pi = (idx_t *)realloc(pi_arr, (new_lim + 1) * sizeof(idx_t));
    if (!new_pi)
        exit(1);
    pi_arr = new_pi;

    /* Obtain all primes up to new_lim */
    uint64_t *tmp_primes = NULL;
    size_t    tmp_cnt    = 0;
    build_primes_upto(new_lim, &tmp_primes, &tmp_cnt);

    /* Store them in primes array */
    free(primes);
    primes = (uint64_t *)malloc(tmp_cnt * sizeof(uint64_t));
    if (!primes)
        exit(1);
    memcpy(primes, tmp_primes, tmp_cnt * sizeof(uint64_t));
    primes_cnt = tmp_cnt;
    free(tmp_primes);

    /* Build pi_arr */
    size_t pi_idx = 0;
    for (idx_t x = 0; x <= new_lim; ++x) {
        while (pi_idx < primes_cnt && primes[pi_idx] <= (uint64_t)x)
            ++pi_idx;
        pi_arr[x] = (idx_t)pi_idx;
    }
    maxN = new_lim;
}

/* ------------------------------------------------------------------
   Recursively search all divisor pairs of sigma.  Returns true if
   a valid prime pair (p,q) with p+q=n and sigma(n)=pi(p)*pi(q) is found.
------------------------------------------------------------------*/
static bool check_divisors_rec(uint64_t sigma,
                               const uint64_t *fac_pr,
                               const uint64_t *fac_exp,
                               size_t fac_cnt,
                               size_t idx,
                               uint64_t cur,
                               idx_t n,
                               idx_t pi_n)
{
    if (idx == fac_cnt) {
        uint64_t d = cur;
        uint64_t e = sigma / d;
        if (d == e)
            return false;
        if (d > pi_n || e > pi_n)
            return false;

        uint64_t p = primes[d - 1];
        uint64_t q = primes[e - 1];
        if (p != q && p + q == (uint64_t)n)
            return true;
        return false;
    }

    uint64_t base = fac_pr[idx];
    uint64_t val  = 1;
    for (uint64_t exp = 0; exp <= fac_exp[idx]; ++exp) {
        if (check_divisors_rec(sigma, fac_pr, fac_exp, fac_cnt,
                               idx + 1, cur * val, n, pi_n))
            return true;
        val *= base;
    }
    return false;
}

/* ------------------------------------------------------------------
   Generate solutions until we have at least 'needed' terms.
------------------------------------------------------------------*/
static void generate_solutions_until(size_t needed)
{
    if (needed <= sol_cnt)
        return;

    /* Ensure initial tables exist */
    if (maxN == 0)
        extend_limit(1000);

    while (sol_cnt < needed) {
        if (next_test > maxN) {
            idx_t new_lim = maxN * 2 + 1000;
            extend_limit(new_lim);
        }

        seq_t sigma_seq = sigma_of(next_test);
        uint64_t sigma = (uint64_t)sigma_seq;
        if (sigma >= 4) {
            uint64_t fac_pr[20];
            uint64_t fac_exp[20];
            size_t    fac_cnt = 0;
            factor_u64(sigma, fac_pr, fac_exp, &fac_cnt);

            if (check_divisors_rec(sigma, fac_pr, fac_exp,
                                   fac_cnt, 0, 1,
                                   next_test, pi_arr[next_test])) {
                /* Store solution */
                if (sol_cnt >= sol_cap) {
                    sol_cap = sol_cap == 0 ? 128 : sol_cap * 2;
                    seq_t *new_sol = (seq_t *)realloc(solutions,
                                                      sol_cap * sizeof(seq_t));
                    if (!new_sol)
                        exit(1);
                    solutions = new_sol;
                }
                solutions[sol_cnt++] = (seq_t)next_test;
            }
        }
        next_test += 2; /* only even numbers can be solutions */
    }
}

/* ------------------------------------------------------------------
   Public function required by the template.
------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    generate_solutions_until((size_t)n);
    return solutions[n - 1];
}
