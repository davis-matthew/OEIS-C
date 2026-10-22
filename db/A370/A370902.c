/*
 * A370902  Partial sums of the powerful part function (A057521).
 * https://oeis.org/A370902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} A057521(k) where A057521(k) = product of
 *            primes p^e with e ≥ 2 in the factorisation of k, or 1 if k is
 *            square‑free
 * Range:     u8 n <= 48, u16 n <= 1649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_alloc, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_spf.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t powerful_part(idx_t n, const idx_t *spf)
{
    seq_t prod = 1;
    while (n > 1) {
        idx_t prime = spf[n];
        idx_t exp   = 0;
        while (n % prime == 0) {
            n /= prime;
            exp++;
        }
        if (exp >= 2) {
            prod *= seq_pow((seq_t)prime, exp);
        }
    }
    return prod;
}

/* ----------  memoised cumulative sums  ---------- */
static seq_t *cum          = NULL;   /* cum[i] = a(i) */
static size_t cum_size     = 0;      /* allocated size */
static idx_t  last_done    = 0;      /* highest index already computed */

static idx_t *spf          = NULL;  /* smallest prime factor table */
static idx_t  spf_limit    = 0;      /* size of spf array */

/* Ensure spf table covers 1..n and cum contains a(n). */
static void assure_upto(idx_t n)
{
    if (n == 0) return;

    /* Build or extend the spf table */
    if (spf_limit < n) {
        if (spf) free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    /* Allocate or grow the cumulative array */
    if (cum_size < (size_t)(n + 1)) {
        if (cum == NULL)
            cum = seq_alloc(n + 1);
        else
            cum = seq_realloc(cum, cum_size, n + 1);
        cum_size = n + 1;
        cum[0] = 0;                     /* initialise first element */
    }

    /* Compute missing cumulative values */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        cum[i] = cum[i - 1] + powerful_part(i, spf);
    }
    last_done = n;
}

seq_t a(idx_t n)
{
    assure_upto(n);
    return cum[n];
}
