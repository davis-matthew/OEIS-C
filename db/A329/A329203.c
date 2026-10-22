/*
 * A329203  Numbers k such that A329308(k) = 0.
 * https://oeis.org/A329203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A329203:  numbers k such that for every integer m with 2 ≤ m < √k,
    the remainder k mod m^2 is not prime.
    This implementation finds the nth term by searching candidates sequentially.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime_small.h"

/* Cached list of found terms (stored as k values). */
static idx_t   *terms   = NULL;   /* array of idx_t values */
static size_t   terms_cap = 0;    /* allocated capacity   */
static size_t   terms_len = 0;    /* number of terms stored */

/* Last candidate considered by the search; starts at 1. */
static idx_t   last_k = 1;

/* Append a newly found term k to the cache. */
static void add_term(idx_t k)
{
    if (terms_len == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 1 : terms_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(terms, new_cap * sizeof(uint64_t));
        /* realloc is guaranteed to succeed for these small sizes in the tests; */
        terms = (idx_t *)tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = k;
}

/* Check whether k satisfies the definition. */
static bool is_good(idx_t k)
{
    /* largest m with m < sqrt(k) */
    idx_t mmax = _floor_isqrt_u64(k - 1);
    for (idx_t m = 2; m <= mmax; ++m) {
        uint64_t sq = m * m;           /* m² fits in 64 bits for k < 2¹²⁸ in practice */
        uint64_t r   = k % sq;
        if (is_prime_small((seq_t)r))
            return false;
    }
    return true;
}

/* Public function: return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1 */

    /* Generate terms until we have at least n terms. */
    for (idx_t k = last_k; terms_len < (size_t)n; ++k) {
        if (is_good(k))
            add_term(k);
    }

    /* Update last_k for the next call. */
    last_k = terms[terms_len - 1] + 1;

    return (seq_t)terms[n - 1];
}
