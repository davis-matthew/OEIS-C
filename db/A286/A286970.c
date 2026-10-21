/*
 * A286970  Number of compositions (ordered partitions) of n into decimal
 *          palindromic primes (A002385).
 * https://oeis.org/A286970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of compositions of n into decimal palindromic
 *            primes
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_palindrome, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (computed by dynamic programming):                 */
/* a(0) = 1, for n > 0: a(n) = Σ_{p ∈ PalPrimes, p ≤ n} a(n‑p)   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
/* Helper functions provided by the build system */
#include "../utils/is_prime_small.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* ---- memoization tables ------------------------------------------ */
    static seq_t *dp        = NULL;   /* DP array with a(i) values   */
    static size_t dp_cap    = 0;      /* current capacity (bytes)    */
    static idx_t  dp_max    = 0;      /* largest computed index      */

    static uint64_t *pal_primes   = NULL; /* list of palindromic primes */
    static size_t   pal_cap        = 0;   /* capacity of above list      */
    static size_t   pal_size       = 0;   /* current number of primes    */
    static idx_t    pal_checked_upto = 1;/* largest number already examined */

    /* ---- initial allocation ------------------------------------------- */
    if (dp_cap == 0) {
        dp_cap = 1;
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* allocation failure, safe fallback */
        dp[0] = (seq_t)1;                  /* a(0) = 1 */
        dp_max = 0;
    }

    /* ---- extend palindromic prime list up to n ----------------------- */
    if (pal_checked_upto < n) {
        for (idx_t cand = pal_checked_upto + 1; cand <= n; ++cand) {
            if (is_prime_small((seq_t)cand) && is_palindrome((seq_t)cand, 10)) {
                if (pal_cap == 0) {
                    pal_cap = 8;
                    pal_primes = (uint64_t *)malloc(pal_cap * sizeof(uint64_t));
                    if (!pal_primes) return (seq_t)0;
                } else if (pal_size == pal_cap) {
                    pal_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(pal_primes, pal_cap * sizeof(uint64_t));
                    if (!tmp) return (seq_t)0;
                    pal_primes = tmp;
                }
                pal_primes[pal_size++] = (uint64_t)cand;
            }
        }
        pal_checked_upto = n;
    }

    /* ---- extend DP array up to n ------------------------------------- */
    if (n > dp_max) {
        if ((size_t)(n + 1) > dp_cap) {
            size_t new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            dp = tmp;
            /* initialise new slots to 0 */
            for (idx_t i = dp_cap; i <= n; ++i)
                dp[i] = (seq_t)0;
            dp_cap = new_cap;
        }
        for (idx_t i = dp_max + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (size_t j = 0; j < pal_size && (idx_t)pal_primes[j] <= i; ++j) {
                idx_t idx = i - (idx_t)pal_primes[j];
                sum += dp[idx];
            }
            dp[i] = sum;
        }
        dp_max = n;
    }

    return dp[n];
}
