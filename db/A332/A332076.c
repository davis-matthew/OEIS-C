/*
 * A332076  Indices n of odd numbers 2n+1 such that k + 2^m is prime, where k
 *          and m are the odd part and 2-valuation, respectively, of 2n.
 * https://oeis.org/A332076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer n such that if 2n = k·2^m with k odd and
 *            m ≥ 1, then k + 2^m is prime. The function returns the n‑th such
 *            n
 * Range:     u8 n <= 62, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

static seq_t *indices = NULL;          /* cache of values satisfying the property */
static size_t  count   = 0;            /* number of cached values */
static size_t  cap     = 0;            /* allocated capacity of the cache */
static idx_t   last_checked = 0;       /* largest n examined so far */

/* Ensure the cache has at least ‘needed’ elements.  */
static void ensure_capacity(size_t needed)
{
    if (needed > cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < needed) newcap <<= 1;
        seq_t *newp = (seq_t *)realloc(indices, newcap * sizeof(seq_t));
        if (!newp) abort();            /* memory exhausted */
        indices = newp;
        cap     = newcap;
    }
}

/* Test whether 2n = k·2^m satisfies k + 2^m prime.  */
static bool property_holds(idx_t n)
{
    /* n ≥ 1 */
    /* trailing zeros in n give v₂(n) = t; then v₂(2n) = t + 1. */
    unsigned t = __builtin_ctzll((unsigned long long)n);
    idx_t m   = (idx_t)t + 1;      /* exponent of the power of 2 in 2n */
    idx_t k   = n >> t;            /* odd part of n */

    seq_t sum = ((seq_t)k) + pow2((idx_t)m);  /* k + 2^m */
    return is_prime_big(sum);
}

/* Return the n‑th index satisfying the condition.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is defined for n ≥ 1 */

    while (count < n) {
        ++last_checked;           /* next integer to test */
        if (property_holds(last_checked)) {
            ensure_capacity(count + 1);
            indices[count++] = (seq_t)last_checked;
        }
    }
    return indices[n - 1];
}
