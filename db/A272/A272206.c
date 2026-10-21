/*
 * A272206  Rounded barycenter of first n primes defined as a(n) =
 *          round(sum_{i=1..n}(i*prime(i)) / sum_{i=1..n}prime(i)).
 * https://oeis.org/A272206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A272206 – Rounded barycenter of first n primes
 * a(n) = round( sum_{i=1..n} i*prime(i) / sum_{i=1..n} prime(i) )
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for memoisation ------------------------------------- */
    static idx_t          max_n   = 0;   /* highest n already processed    */
    static seq_t          *sum_pr  = NULL; /* cumulative sum of primes       */
    static seq_t          *sum_wt  = NULL; /* cumulative sum of i*prime(i)   */
    static uint32_t       *primes  = NULL; /* list of primes                 */
    static size_t         p_cap    = 0;    /* capacity of primes array       */

    /* ------------------------------------------------------------------ */
    if (max_n < n) { /* need to compute up to n                                 */
        /* enlarge arrays to hold sums up to n+1 (index 0 unused)          */
        size_t new_cap = (size_t)n + 1;

        sum_pr = (seq_t *)realloc(sum_pr, new_cap * sizeof(seq_t));
        sum_wt = (seq_t *)realloc(sum_wt, new_cap * sizeof(seq_t));
        if (!sum_pr || !sum_wt)
            return 0; /* allocation failure – unlikely for contest data */

        /* initialise cumulative sums for indices > max_n              */
        if (!sum_pr[0]) sum_pr[0] = 0;      /* zero the 0‑th entry if not set */
        if (!sum_wt[0]) sum_wt[0] = 0;

        /* ensure capacity for primes array                            */
        if (p_cap < (size_t)n + 10) {
            size_t new_p_cap = (p_cap ? p_cap * 2 : 64);
            if (new_p_cap < (size_t)n + 10) new_p_cap = n + 10;
            uint32_t *tmp = (uint32_t *)realloc(primes, new_p_cap * sizeof(uint32_t));
            if (!tmp) return 0;
            primes = tmp;
            p_cap  = new_p_cap;
        }

        /* generate primes and update running sums                     */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            uint32_t candidate;
            if (i == 1u) {
                candidate = 2u;
            } else {
                candidate = primes[i - 2] + 1u;
                /* test divisibility by known primes up to sqrt(candidate) */
                for (;;) {
                    bool composite = false;
                    for (size_t j = 0; j < (size_t)(i - 1); ++j) {
                        uint32_t p = primes[j];
                        uint64_t sq = (uint64_t)p * (uint64_t)p;
                        if (sq > candidate) break;
                        if (candidate % p == 0) {
                            composite = true;
                            break;
                        }
                    }
                    if (!composite) break; /* prime found                 */
                    ++candidate;           /* try next */
                }
            }
            primes[i - 1] = candidate;

            sum_pr[i] = sum_pr[i - 1] + (seq_t)candidate;
            sum_wt[i] = sum_wt[i - 1] + (seq_t)i * (seq_t)candidate;
        }

        max_n = n;
    }

    /* ------------------------------------------------------------------ */
    /* compute rounded barycenter for n                                 */
    seq_t den = sum_pr[n];      /* sum of primes                      */
    seq_t num = sum_wt[n];      /* sum of i*prime(i)                  */
    seq_t q   = num / den;      /* floor division                      */
    seq_t r   = num % den;      /* remainder                           */
    if ((r << 1) >= den) ++q;   /* round to nearest integer            */

    return q;
}
