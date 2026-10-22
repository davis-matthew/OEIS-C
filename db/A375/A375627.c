/*
 * A375627  Expansion of e.g.f. exp(2*x^2) / (1 - x * exp(x^2)).
 * https://oeis.org/A375627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} (n-2*k+2)^k / k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *fac_cache = NULL;   /* factorials fac[n] = n! */
static size_t fac_cap   = 0;      /* number of factorials stored */

static seq_t *seq_cache = NULL;   /* sequence values a[n] */
static size_t seq_cap   = 0;      /* number of sequence values stored */

/* Ensure factorial cache contains fac[0] … fac[n] */
static void
ensure_factorials(idx_t n)
{
    if (fac_cap > (size_t)n)
        return;

    size_t needed = (size_t)n + 1;
    seq_t *new_fac = (seq_t*)realloc(fac_cache, needed * sizeof(seq_t));
    if (!new_fac) abort();
    fac_cache = new_fac;

    if (fac_cap == 0) {
        fac_cache[0] = 1;
        fac_cap = 1;
    }
    for (size_t i = fac_cap; i < needed; ++i)
        fac_cache[i] = fac_cache[i - 1] * (seq_t)i;

    fac_cap = needed;
}

/* Compute a single term (n! / k!) * (n-2*k+2)^k */
static seq_t
term_value(idx_t n, idx_t k)
{
    seq_t term = fac_cache[n] / fac_cache[k];          /* n! / k!  (exact division) */
    seq_t base  = (seq_t)(n - 2 * k + 2);
    seq_t powv  = 1;
    for (idx_t e = 0; e < k; ++e)
        powv *= base;
    term *= powv;
    return term;
}

/* Compute a(n) when it has not been cached yet */
static void
ensure_sequence(idx_t n)
{
    if (seq_cap > (size_t)n)
        return;

    size_t needed = (size_t)n + 1;
    seq_t *new_seq = (seq_t*)realloc(seq_cache, needed * sizeof(seq_t));
    if (!new_seq) abort();
    seq_cache = new_seq;

    ensure_factorials(n);          /* make sure factorials up to n exist */

    for (size_t i = seq_cap; i < needed; ++i) {
        idx_t nn = (idx_t)i;
        idx_t maxk = nn / 2;
        seq_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k)
            sum += term_value(nn, k);
        seq_cache[i] = sum;
    }

    seq_cap = needed;
}

seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq_cache[n];
}
