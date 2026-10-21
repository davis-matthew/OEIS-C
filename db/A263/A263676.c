/*
 * A263676  Numbers that are both interprime and oblong.
 * https://oeis.org/A263676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th number that is both an oblong number m*(m+1) and
 *            an interprime (average of two consecutive primes)
 * Range:     u8 n <= 7, u16 n <= 61, u32 n <= 5552, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Greatest prime <= n (n >= 1). */
static seq_t prev_prime_leq(seq_t n)
{
    if (n <= 2)
        return 2;

    /* Make odd if even. */
    if ((n & 1U) == 0)
        n -= 1;

    while (n >= 2) {
        if (isPrimeGeneric(n))
            return n;
        n -= 2;
    }
    return 2;  /* should not reach */
}

/* Smallest prime > n (n >= 1). */
static seq_t next_prime_gt(seq_t n)
{
    seq_t candidate = n + 1;
    if (candidate <= 2)
        return 2;
    if ((candidate & 1U) == 0)
        candidate += 1;
    while (true) {
        if (isPrimeGeneric(candidate))
            return candidate;
        candidate += 2;
    }
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used but defined for safety */
        return 0;

    /* Static cache of found terms. */
    static seq_t *terms      = NULL;
    static size_t terms_len  = 0;
    static size_t terms_cap  = 0;
    static idx_t  last_m     = 1;   /* next m to examine */

    if (terms == NULL) {          /* first-time initialization */
        terms_cap = 64;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < n) {
        /* Next candidate m */
        ++last_m;
        seq_t m = (seq_t)last_m;
        seq_t B = m * (m + 1);          /* oblong m*(m+1) */

        /* p: largest prime < B */
        seq_t p = prev_prime_leq(B - 1);
        /* q: smallest prime > p */
        seq_t q = next_prime_gt(p);

        /* Check that p and q are consecutive and average to B */
        seq_t sum_pq = p + q;
        seq_t double_B = (seq_t)2 * B;
        if (sum_pq == double_B) {
            /* Store B */
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = B;
        }
    }

    return terms[n - 1];
}
