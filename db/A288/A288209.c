/*
 * A288209  Numbers k such that prime(k) * prime(k+1) mod prime(k+2) is odd.
 * https://oeis.org/A288209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288209: numbers k such that prime(k)*prime(k+1) mod prime(k+2) is odd. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static seq_t *terms = NULL;       /* cache of found k's */
static size_t  terms_len = 0;     /* number of terms stored */
static size_t  terms_cap = 0;     /* capacity of the cache */
static idx_t   next_k   = 1;      /* next prime index to test */

/* Ensure cache has at least 'need' capacity. */
static void ensure_cap(size_t need)
{
    if (terms_cap >= need) return;
    if (terms_cap == 0) terms_cap = 8;
    while (terms_cap < need) terms_cap <<= 1;
    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
}

/* Main function providing the n-th element of the sequence. */
seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is the first term. */
    if (n == 0) return 0;          /* undefined, but return 0 as safe fallback */

    /* Expand cache until we have at least n terms. */
    while (terms_len < n)
    {
        idx_t k = next_k++;                     /* candidate prime index */
        uint32_t pk  = get_prime(k);
        uint32_t pk1 = get_prime(k + 1);
        uint32_t pk2 = get_prime(k + 2);

        /* Compute (pk * pk1) mod pk2; use 64‑bit to avoid overflow. */
        uint64_t mod = ((uint64_t)pk * pk1) % pk2;

        if ((mod & 1ULL) != 0)   /* odd? */
        {
            ensure_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
