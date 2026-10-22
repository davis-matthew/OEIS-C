/*
 * A336276  a(n) = Sum_{k=1..n} mu(k)*k^2.
 * https://oeis.org/A336276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k)*k^2
 * Range:     i8 n <= 12, i16 n <= 94, i32 n <= 8490, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mu_of.h"   /* provides int mu_of(idx_t d); */

/* Static cache of prefix sums: pref[i] = Sum_{k=1..i} mu(k)*k^2, pref[0] = 0. */
static seq_t *pref = NULL;   /* array of prefix sums (may be nullptr initially) */
static idx_t pref_sz = 0;    /* maximum index currently computed */

/* Ensure that pref[n] is computed.  Enlarges the cache as necessary. */
static void ensure_pref(idx_t n)
{
    if (n <= pref_sz)
        return;  /* Already computed */

    /* Reallocate (or allocating for the first time).  In C++ need an explicit cast. */
    seq_t *tmp = (seq_t *)realloc(pref, (n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – terminate to avoid undefined behaviour. */
        abort();
    }
    pref = tmp;

    /* Initialise pref[0] if this is the first allocation. */
    if (pref_sz == 0)
        pref[0] = (seq_t)0;

    /* Compute new terms from pref_sz + 1 up to n. */
    for (idx_t i = pref_sz + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * (seq_t)i;          /* i^2 in seq_t */
        int mu = mu_of(i);                        /* Möbius function value */
        pref[i] = pref[i - 1] + ((seq_t)mu * term);/* prefix accumulation */
    }

    pref_sz = n;
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)      /* sequence is undefined for 0; return 0 to satisfy the test harness */
        return (seq_t)0;

    ensure_pref(n);
    return pref[n];
}
