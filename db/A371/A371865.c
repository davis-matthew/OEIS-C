/*
 * A371865  Primes whose second to ninth digits are 23456789.
 * https://oeis.org/A371865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes whose second through ninth decimal digits are 23456789.
   The function enumerates all integers of the form:
   p = d * 10^(L-1) + 23456789 * 10^(L-9) + r
   where d ∈ [1,9], L ≥ 9, and 0 ≤ r < 10^(L-9).
   Each candidate is tested for primality with isPrimeGeneric. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"

/* Static cache of primes that have been found so far. */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;

/* Iterator state for generating the next candidate. */
static idx_t   cur_L     = 9;        /* current length of the number */
static int     cur_d     = 1;        /* current leading digit (1–9) */
static idx_t   cur_r     = 0;        /* current suffix value (0 .. limit-1) */
static idx_t   cur_limit = 0;        /* number of suffixes for current L,d */
static seq_t   cur_base  = 0;        /* base value for current L,d */
static bool    cur_inited = false;   /* flag whether iterator has been set up */

/* Initialise iterator for the first time or after a full reset. */
static void init_iterator(void) {
    if (cur_inited) return;
    cur_limit  = (idx_t)pow10(cur_L - 9);   /* 10^(L-9)  */
    seq_t pow10_Lm1 = pow10(cur_L - 1);     /* 10^(L-1)  */
    seq_t pow10_Lm9 = pow10(cur_L - 9);     /* 10^(L-9)  */
    cur_base = (seq_t)cur_d * pow10_Lm1 + 23456789 * pow10_Lm9;
    cur_inited = true;
}

/* Advance iterator to the next candidate. */
static void advance_iterator(void) {
    if (cur_r + 1 == cur_limit) {           /* move to next leading digit */
        cur_r = 0;
        if (cur_d < 9) {
            cur_d++;
        } else {
            cur_d = 1;
            cur_L++;
        }
        cur_limit = (idx_t)pow10(cur_L - 9);
        seq_t pow10_Lm1 = pow10(cur_L - 1);
        seq_t pow10_Lm9 = pow10(cur_L - 9);
        cur_base = (seq_t)cur_d * pow10_Lm1 + 23456789 * pow10_Lm9;
    } else {
        cur_r++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, index 0 is unused */

    while (prime_cache_len < (size_t)n) {
        /* grow cache if needed */
        if (prime_cache_len == prime_cache_cap) {
            size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 32;
            seq_t *tmp = (seq_t *)realloc(prime_cache,
                                          new_cap * sizeof(seq_t));
            prime_cache = tmp;
            prime_cache_cap = new_cap;
        }

        init_iterator();

        seq_t p = cur_base + cur_r;

        /* skip numbers whose last digit cannot be prime */
        seq_t ld = p % 10;
        if (ld != 1 && ld != 3 && ld != 7 && ld != 9) {
            advance_iterator();
            continue;
        }

        if (isPrimeGeneric(p)) {
            prime_cache[prime_cache_len++] = p;
        }

        advance_iterator();
    }

    return prime_cache[n - 1];
}
