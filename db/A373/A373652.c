/*
 * A373652  Composite numbers k for which g = gcd(f(i*c), k) = 1 or k for all
 *          i in the range 1 <= i <= c, where f(x) = Product_{j=1..c} x+j and
 *          c = floor(k^(1/4)).
 * https://oeis.org/A373652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt, gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373652: composite numbers k such that
 * let c = ⌊k^(1/4)⌋.  For every i = 1..c,
 *   g = gcd(∏_{j=1..c}(i*c+j), k)
 * is either 1 or k.
 * The implementation performs an on‑demand search with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* Test whether a given composite k satisfies the required property. */
static bool ok_k(seq_t k)
{
    /* Reject primes */
    if (is_prime_big(k))
        return false;

    /* Compute c = floor(k^(1/4)) = floor(sqrt(floor_sqrt(k))). */
    seq_t s1 = floor_sqrt(k);
    seq_t c  = floor_sqrt(s1);
    if (c == (seq_t)0)
        return false;

    /* For cast safety, ensure c fits into idx_t for the loops. */
    if (c > (seq_t)((idx_t)-1))
        return false;
    idx_t c_idx = (idx_t)c;

    for (idx_t i = 1; i <= c_idx; ++i)
    {
        seq_t base = (seq_t)i * c;
        seq_t prod = 1;

        for (idx_t j = 1; j <= c_idx; ++j)
        {
            seq_t term = base + (seq_t)j;
            prod = (prod * term) % k;   /* product modulo k */
        }

        seq_t g = gcd(prod, k);
        if (g != (seq_t)1 && g != k)
            return false;
    }
    return true;
}

/* Global cache for found sequence terms. */
static seq_t *terms    = NULL;
static size_t term_cnt = 0;
static size_t term_cap = 0;
static seq_t next_k = 4;          /* start checking after 3 */

/* Ensure at least n terms have been computed and cached. */
static void need_terms(idx_t n)
{
    while (term_cnt < n)
    {
        seq_t k = next_k++;

        if (!ok_k(k))
            continue;

        if (term_cnt == term_cap)
        {
            term_cap = (term_cap == 0 ? 16 : term_cap * 2);
            /* cast needed for C++ */
            seq_t *tmp = (seq_t*)realloc(terms, term_cap * sizeof(seq_t));
            if (!tmp)
                return;          /* allocation failed – abort further work */
            terms = tmp;
        }
        terms[term_cnt++] = k;
    }
}

/* Public function required by the platform. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined behaviour for n < 1 */
    need_terms(n);
    return terms[n - 1];
}
