/*
 * A370523  Numbers k > 2 such that all positive values of k - 2^(2^m) are
 *          prime, with integer m >= 0.
 * https://oeis.org/A370523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370523:
   Numbers k > 2 such that for every m ≥ 0 with 2^(2^m) < k,
   the difference k – 2^(2^m) is prime.  The function searches for
   such numbers incrementally and caches already found terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *terms   = NULL;   /* cache of found terms               */
static size_t terms_cap = 0;    /* allocated capacity of the cache    */
static size_t terms_len = 0;    /* current number of stored terms     */
static seq_t last_k = 3;        /* last k that was tested             */

static void ensure_terms_cap(size_t need)
{
    if (need > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

static void add_term(seq_t v)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = v;
}

/* Test whether a candidate k satisfies the definition.  */
static bool is_valid(seq_t k)
{
    if (k <= 2) return false;

    seq_t p = 2;                           /* 2^(2^0) */
    while (p < k) {
        seq_t diff = k - p;
        if (!is_prime_big(diff)) return false;
        /* next power: (2^(2^m))^2 = 2^(2^(m+1)) */
        if (p > k / p) break;             /* next square would be ≥ k */
        p = p * p;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        ++last_k;
        if (is_valid(last_k))
            add_term(last_k);
    }
    return terms[n - 1];
}
