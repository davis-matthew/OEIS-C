/*
 * A382739  a(n) = Sum_{k=0..n} (k!)^2 * binomial(k+3,3) * Stirling2(n,k)^2.
 * https://oeis.org/A382739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382739
   Formula: a(n) = Sum_{k=0..n} (k!)^2 * binomial(k+3,3) * Stirling2(n,k)^2
   in which Stirling2(n,k) is the Stirling number of the second kind. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* seq_t and idx_t are provided by the build system */

seq_t a(idx_t n)
{
    /* ----- static state ----- */
    static bool initialized = false;          /* have we allocated state? */
    static idx_t max_computed = 0;            /* highest n for which a(n) is computed */

    /* cache of a(n) values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* factorials k! for k up to current n */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;

    /* binomial(k+3,3) for k up to current n */
    static seq_t *binom3 = NULL;
    static size_t binom3_cap = 0;

    /* Stirling numbers of the second kind, current n */
    static seq_t *S_cur = NULL;
    static size_t S_cur_cap = 0;
    static seq_t *S_next = NULL;
    static size_t S_next_cap = 0;

    /* ---------- initial allocation ---------- */
    if (!initialized) {
        /* allocate for at least the requested n */
        idx_t target = n;

        /* cache */
        cache_cap = target + 1;
        cache = seq_realloc(NULL, 0, cache_cap);

        /* factorials */
        fact_cap = target + 1;
        fact = seq_realloc(NULL, 0, fact_cap);
        fact[0] = (seq_t)1;                 /* 0! = 1 */

        /* binomial(k+3,3) */
        binom3_cap = target + 1;
        binom3 = seq_realloc(NULL, 0, binom3_cap);
        binom3[0] = (seq_t)1;                /* binom(3,3) = 1 */

        /* fill factorials and binom3 up to target */
        for (idx_t k = 1; k <= target; ++k) {
            fact[k] = fact[k - 1] * (seq_t)k;

            seq_t t = (seq_t)(k + 1);
            t *= (k + 2);
            t *= (k + 3);
            t /= 6;
            binom3[k] = t;
        }

        /* Stirling arrays */
        S_cur_cap = 1;
        S_cur = seq_realloc(NULL, 0, S_cur_cap);
        S_cur[0] = (seq_t)1;   /* S(0,0) = 1 */

        S_next_cap = 0;
        S_next = NULL;

        /* base case */
        cache[0] = (seq_t)1;   /* a(0) */
        max_computed = 0;
        initialized = true;
    }

    /* ---------- grow to requested n (if needed) ---------- */
    if (n > max_computed) {
        idx_t target = n;

        /* enlarge cache if necessary */
        if (cache_cap < target + 1) {
            cache = seq_realloc(cache, cache_cap, target + 1);
            cache_cap = target + 1;
        }

        /* enlarge factorial array if necessary */
        if (fact_cap < target + 1) {
            size_t old_fact_cap = fact_cap;
            fact = seq_realloc(fact, fact_cap, target + 1);
            fact_cap = target + 1;
            for (idx_t k = old_fact_cap; k <= target; ++k) {
                fact[k] = fact[k - 1] * (seq_t)k;
            }
        }

        /* enlarge binomial(k+3,3) array if necessary */
        if (binom3_cap < target + 1) {
            size_t old_binom3_cap = binom3_cap;
            binom3 = seq_realloc(binom3, binom3_cap, target + 1);
            binom3_cap = target + 1;
            for (idx_t k = old_binom3_cap; k <= target; ++k) {
                seq_t t = (seq_t)(k + 1);
                t *= (k + 2);
                t *= (k + 3);
                t /= 6;
                binom3[k] = t;
            }
        }

        /* enlarge Stirling arrays if needed */
        if (S_cur_cap < target + 1) {
            S_cur = seq_realloc(S_cur, S_cur_cap, target + 1);
            S_cur_cap = target + 1;
        }
        if (S_next_cap < target + 1) {
            S_next = seq_realloc(S_next, S_next_cap, target + 1);
            S_next_cap = target + 1;
        }

        /* compute values for all n > max_computed up to target */
        for (idx_t current = max_computed + 1; current <= target; ++current) {
            /* compute S_next for the current n */
            S_next[0] = (seq_t)0;   /* S(n,0) = 0 for n>0 */
            for (idx_t k = 1; k <= current; ++k) {
                seq_t val = (seq_t)0;
                if (k - 1 <= max_computed)        /* S(n-1,k-1) */
                    val += S_cur[k - 1];
                if (k <= max_computed)            /* k * S(n-1,k) */
                    val += (seq_t)k * S_cur[k];
                S_next[k] = val;
            }

            /* compute a(current) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= current; ++k) {
                seq_t term = fact[k];
                term *= fact[k];                /* (k!)^2 */
                term *= binom3[k];               /* * binomial(k+3,3) */
                seq_t sk = S_next[k];
                sk *= sk;                        /* * Stirling^2 */
                term *= sk;
                sum += term;
            }
            cache[current] = sum;

            /* prepare for next iteration */
            seq_t *tmp = S_cur;
            S_cur = S_next;
            S_next = tmp;

            max_computed = current;
        }
    }

    /* return the already computed value */
    return cache[n];
}
