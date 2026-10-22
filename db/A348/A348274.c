/*
 * A348274  Noninfinitary abundant numbers: numbers k such that A348271(k) >
 *          k.
 * https://oeis.org/A348274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 2933, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Noninfinitary abundant numbers:
   a(n) = the n‑th integer k such that the sum of all non‑infinitary
   divisors of k exceeds k. (A348274) */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* utilities supplied by the build system */
#include "../utils/seq_pow.h"        /* seq_pow(base, exp)         */
#include "../utils/build_spf.h"      /* build_spf(limit, &spfs)    */
#include "../utils/sigma_of.h"       /* sigma_of(n)                */

/* static state */
static idx_t max_limit  = 0;         /* highest n that has been
                                        fully processed                */
static idx_t *spf        = NULL;     /* smallest prime factor array    */
static seq_t *sigma_inf  = NULL;     /* σ_inf[n] for n ≤ max_limit    */
static seq_t *terms      = NULL;     /* list of noninfinitary abundant
                                        numbers                     */
static size_t terms_len   = 0;        /* number of terms stored        */
static size_t terms_cap   = 0;        /* allocated capacity for terms   */

/* enlarge the terms array when necessary */
static void grow_terms(void)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 1024 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

/* build σ_inf for all numbers in (max_limit, new_limit] */
static void extend_to(idx_t new_limit)
{
    if (new_limit <= max_limit) return;

    /* rebuild small‑prime‑factor table up to new_limit */
    build_spf(new_limit, &spf);
    if (!spf) return;              /* allocation error – abort quietly */

    /* enlarge σ_inf array */
    seq_t *tmp = (seq_t *)realloc(sigma_inf, (new_limit + 1) * sizeof(seq_t));
    if (!tmp) return;              /* allocation error – abort quietly */
    sigma_inf = tmp;
    sigma_inf[1] = (seq_t)1;      /* σ_inf(1) = 1 */

    /* process each new integer */
    for (idx_t i = max_limit + 1; i <= new_limit; ++i) {
        idx_t n = i;
        seq_t prod_inf = (seq_t)1; /* product of (1 + p^(2^bit))   */
        seq_t total    = (seq_t)1; /* product of σ(p^e)              */

        /* factorise n using the SPF table */
        while (n > 1) {
            idx_t p = spf[n];
            idx_t e = 0;
            while ((n % p) == 0) {
                n /= p;
                ++e;
            }

            /* contribution to σ_inf from this prime   */
            seq_t factor_inf = (seq_t)1;
            idx_t bit = 0, exp = e;
            while (exp != 0) {
                if (exp & 1u) {
                    seq_t pw = seq_pow((seq_t)p, (idx_t)1ULL << bit);
                    factor_inf *= (pw + (seq_t)1);
                }
                ++bit;
                exp >>= 1;
            }
            prod_inf *= factor_inf;

            /* contribution to σ (ordinary sum of divisors) */
            seq_t pw = seq_pow((seq_t)p, e + 1);
            seq_t prime_sigma = (pw - (seq_t)1) / ((seq_t)p - (seq_t)1);
            total *= prime_sigma;
        }

        sigma_inf[i] = prod_inf;

        /* sum of non‑infinitary divisors = σ(n) – σ_inf(n) */
        seq_t non_inf_sum = total - prod_inf;

        if (non_inf_sum > (seq_t)i) {
            grow_terms();
            terms[terms_len++] = (seq_t)i;
        }
    }

    max_limit = new_limit;
}

/* ensure that at least n terms have been computed */
static void ensure_n_terms(idx_t n)
{
    idx_t limit = (max_limit == 0) ? 1 : max_limit;
    while (terms_len < (size_t)n) {
        idx_t new_limit = limit * 2;
        if (new_limit <= limit) new_limit = limit + 1;   /* overflow guard */
        extend_to(new_limit);
        limit = new_limit;
    }
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* OK for safety; not used by OEIS */

    ensure_n_terms(n);
    return terms[n - 1];
}
