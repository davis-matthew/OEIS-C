/*
 * A367585  Numbers k whose multiset multiplicity kernel (in which each prime
 *          exponent becomes the least prime factor with that exponent) is
 *          different from that of all positive integers less than k.
 * https://oeis.org/A367585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   factor_u64, seq_cmp, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A367585: Numbers k whose multiset multiplicity kernel is new.
   MMK(n) = ∏_{exponent e} (least prime factor with exponent e)^(# of primes with exponent e).
   a(n) is the nth integer whose MMK has not appeared before among 1..k-1.
   The function builds the sequence on demand, memoising earlier results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq_arr  = NULL;   /* sequence values (the k's) */
    static size_t seq_cap  = 0, seq_len = 0;
    static seq_t *seen_arr = NULL;   /* MMK values seen so far */
    static size_t seen_cap = 0, seen_len = 0;
    static uint64_t next_k  = 1;     /* next integer to examine */

    /* initialise arrays on first call */
    if (seq_arr == NULL) {
        seq_cap = 1024;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    if (seen_arr == NULL) {
        seen_cap = 1024;
        seen_arr = (seq_t *)malloc(seen_cap * sizeof(seq_t));
    }

    /* generate terms until we have n of them */
    while (seq_len < n) {
        seq_t mmk;
        if (next_k == 1) {
            mmk = 1;               /* kernel of 1 is defined as 1 */
        } else {
            /* factorise next_k */
            uint64_t pr[64], ex[64];
            size_t cnt = 0;
            factor_u64(next_k, pr, ex, &cnt);

            /* gather min prime and count per exponent */
            int min_prime[65] = {0};
            int count[65]     = {0};
            int max_exp = 0;
            for (size_t i = 0; i < cnt; ++i) {
                int e = (int)ex[i];
                uint64_t p = pr[i];
                if (e > max_exp) max_exp = e;
                if (min_prime[e] == 0 || p < (uint64_t)min_prime[e])
                    min_prime[e] = (int)p;
                count[e]++;
            }

            /* build the MMK */
            mmk = 1;
            for (int e = 1; e <= max_exp; ++e) {
                if (count[e] == 0) continue;
                seq_t term = seq_pow((seq_t)min_prime[e], (idx_t)count[e]);
                mmk = seq_mul(mmk, term);
            }
        }

        /* check if this MMK has appeared before */
        bool dup = false;
        for (size_t i = 0; i < seen_len; ++i) {
            if (seq_cmp(&seen_arr[i], &mmk) == 0) {
                dup = true;
                break;
            }
        }

        if (!dup) {
            /* grow sequence array if needed */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
            }
            if (seen_len == seen_cap) {
                seen_cap *= 2;
                seen_arr = (seq_t *)realloc(seen_arr, seen_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = (seq_t)next_k;
            seen_arr[seen_len++] = mmk;
        }

        ++next_k;
    }

    return seq_arr[n - 1];
}
