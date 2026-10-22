/*
 * A307240  a(0) = 1; a(n) = Sum_{k=1..n} -lambda(k+1)*a(n-k), where lambda()
 *          is the Liouville function (A008836).
 * https://oeis.org/A307240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = –Sum_{k=1..n} λ(k+1) * a(n‑k) (λ = Liouville)
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/* Cache for the sequence values                                             */
static seq_t *a_cache = NULL;                    /* a_cache[i] = a(i)          */
static size_t a_cache_len = 0;                   /* number of entries filled   */

/* Cache for Liouville λ values ------------------------------------------- */
static int8_t *lambda_arr = NULL;                /* λ(i) stored as +/-1        */
static size_t lambda_len = 0;                    /* size of lambda_arr         */

/* Smallest prime factor array for computing λ ---------------------------- */
static uint64_t *spf_arr = NULL;                 /* spf(i) gives a prime factor */
static size_t spf_len = 0;                       /* size of spf_arr            */

/* -------------------------------------------------------------------------- */
/* Forward declarations                                                     */
static void compute_sieve(size_t up_to);
static void ensure_cache(idx_t need_len);
static void ensure_lambda(idx_t n);
static seq_t compute_a_at(idx_t n);

/* -------------------------------------------------------------------------- */
/* Sieve for smallest prime factors and Liouville λ values                  */
static void compute_sieve(size_t up_to)
{
    size_t size = up_to + 1;                      /* include index up_to       */
    spf_arr = (uint64_t*)realloc(spf_arr, size * sizeof(uint64_t));
    memset(spf_arr, 0, size * sizeof(uint64_t));

    uint32_t *pr = (uint32_t*)malloc(size * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= up_to; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = (uint64_t)i;
            pr[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt && (idx_t)i * pr[j] <= up_to &&
             pr[j] <= spf_arr[i]; ++j) {
            spf_arr[i * pr[j]] = pr[j];
        }
    }

    lambda_arr = (int8_t*)realloc(lambda_arr, size * sizeof(int8_t));
    lambda_arr[0] = 0;            /* not used                                    */
    lambda_arr[1] = 1;            /* λ(1) = 1                                      */
    for (idx_t i = 2; i <= up_to; ++i) {
        uint64_t p = spf_arr[i];
        lambda_arr[i] = (int8_t)(-lambda_arr[i / p]);
    }

    free(pr);
    spf_len = size;
    lambda_len = size;
}

/* -------------------------------------------------------------------------- */
/* Ensure a_cache holds at least need_len+1 entries                          */
static void ensure_cache(idx_t need_len)
{
    if (a_cache == NULL) {
        a_cache = (seq_t*)malloc((need_len + 1) * sizeof(seq_t));
        a_cache_len = 0;
    } else if (a_cache_len <= (size_t)need_len) {
        a_cache = (seq_t*)realloc(a_cache, (need_len + 1) * sizeof(seq_t));
        a_cache_len = 0;            /* reset because we recomputed all */
    }

    if (a_cache_len == 0) {
        a_cache[0] = (seq_t)1;      /* a(0) = 1 */
        a_cache_len = 1;
    }
}

/* -------------------------------------------------------------------------- */
/* Ensure that λ(k) is known for all k <= n+1                                 */
static void ensure_lambda(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (lambda_len <= needed) {
        compute_sieve(needed);
    }
}

/* -------------------------------------------------------------------------- */
/* Compute a(n) assuming a(0..n‑1) are already in a_cache                    */
static seq_t compute_a_at(idx_t n)
{
    /* a(n) = - Σ_{k=1..n} λ(k+1) * a(n-k)                                         */
    seq_t abs_sum = 0;               /* magnitude of the running sum          */
    bool sum_neg   = false;          /* true if the current sum is negative   */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t term   = a_cache[(size_t)(n - k)];
        bool term_neg = lambda_arr[(size_t)(k + 1)] < 0;   /* λ(k+1) == -1 ? */

        if (!term_neg) {           /* add term */
            if (sum_neg) {
                if (abs_sum > term) {
                    abs_sum -= term;          /* stay negative      */
                } else if (abs_sum < term) {
                    abs_sum = term - abs_sum;
                    sum_neg = false;          /* becomes positive  */
                } else {
                    abs_sum = 0;
                    sum_neg = false;          /* zero              */
                }
            } else {            /* already positive */
                abs_sum += term;
            }
        } else {                   /* subtract term (-λ) */
            if (sum_neg) {
                abs_sum += term;          /* still negative  */
            } else {
                if (abs_sum > term) {
                    abs_sum -= term;          /* remains positive  */
                } else if (abs_sum < term) {
                    abs_sum = term - abs_sum;
                    sum_neg = true;           /* becomes negative  */
                } else {
                    abs_sum = 0;
                }
            }
        }
    }

    /* a(n) = -sum; since sum is negative (almost always) the result is abs_sum   */
    if (sum_neg) {
        return abs_sum;
    } else {
        /* the only way to get here is that the sum is +0 or +positive,
           in which case -sum would be zero.  Returning zero is safe. */
        return (seq_t)0;
    }
}

/* -------------------------------------------------------------------------- */
/* Public function required by the kata                                      */
seq_t a(idx_t n)
{
    ensure_cache(n);
    ensure_lambda(n);

    if (n < a_cache_len) {
        return a_cache[(size_t)n];
    }

    for (idx_t i = a_cache_len; i <= n; ++i) {
        a_cache[(size_t)i] = compute_a_at(i);
    }
    a_cache_len = (size_t)(n + 1);
    return a_cache[(size_t)n];
}
