/*
 * A366186  Positive integers k such that the third derivative of the k-th
 *          Bernoulli polynomial B(k, x) contains only integer coefficients.
 * https://oeis.org/A366186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A366186: Positive integers k such that the third derivative of the k‑th Bernoulli polynomial B(k,x)
 * has all integer coefficients.
 * k is a term iff for all primes p ≤ (k‑1)/(2+((k‑1) mod 2)) with gcd(p,(k)_3)=1 we have
 * the sum of the base‑p digits of (k‑2) < p.  For k ≤ 3 this condition is trivially true.
 * The first few terms are 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,20,21,22,25,26,28,...
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Test whether k satisfies D₃(k)=1. */
static bool is_special(idx_t k)
{
    /* All n ≤ 3 belong to the sequence. */
    if (k <= 3)
        return true;

    /* Compute the bound: floor((k-1)/(2 + ((k-1) mod 2))) */
    idx_t denom = ((k - 1) & 1) ? 3 : 2;   /* 3 if (k-1) odd, 2 otherwise */
    idx_t bound = (k - 1) / denom;

    /* Cache primes up to the current bound. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (bound > prime_limit) {
        if (primes != NULL)
            free(primes);
        build_primes_upto(bound, &primes, &prime_cnt);
        prime_limit = bound;
    }

    /* Check each prime p ≤ bound. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];

        /* Skip if p divides any of k, k-1, k-2. */
        if (k % p == 0 || (k - 1) % p == 0 || (k - 2) % p == 0)
            continue;

        /* Sum of base‑p digits of (k-2). */
        idx_t n2 = k - 2;
        idx_t sum = 0;
        while (n2 > 0) {
            sum += n2 % p;
            n2 /= p;
        }

        /* If the digit sum is at least p, p divides D₃(k) → not a term. */
        if (sum >= p)
            return false;
    }
    return true;
}

/* Return the n‑th term of sequence A366186 (1‑based index). */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static idx_t last_checked = 1;

    /* Initialise the cache on first call. */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        last_checked = 1;
    }

    /* Extend the cached list until we have at least n terms. */
    while (terms_len < n) {
        if (is_special(last_checked)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }

    return terms[n - 1];
}
