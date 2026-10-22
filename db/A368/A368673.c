/*
 * A368673  Number of squarefree numbers less than n that do not divide n.
 * https://oeis.org/A368673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} μ(k)² * (⌈n/k⌉ – ⌊n/k⌋) Implemented by caching
 *            a sieve of square‑free numbers and counting the square‑free
 *            divisors of n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached square‑free flags: 1 if i is square‑free, 0 otherwise. */
static unsigned char *sqf_flag   = NULL;
/* Cached prefix sums of the square‑free counts. */
static seq_t       *sqf_prefix  = NULL;
/* Current capacity of the cached arrays (size of the arrays). */
static size_t       sqf_cap     = 0;

/* Set the cache to hold data up to and including n. */
static void recompute_sqf(idx_t n)
{
    size_t new_cap = (size_t)n + 1;          /* indices 0 .. n */

    /* Reallocate the flag array. */
    unsigned char *flag   = (unsigned char*)realloc(sqf_flag, new_cap * sizeof(unsigned char));
    if (!flag) abort();
    sqf_flag = flag;

    /* Reallocate the prefix array. */
    seq_t *prefix = (seq_t*)realloc(sqf_prefix, new_cap * sizeof(seq_t));
    if (!prefix) abort();
    sqf_prefix = prefix;

    sqf_cap = new_cap;

    /* Initially assume every integer ≥1 is square‑free. */
    for (size_t i = 1; i <= n; ++i) sqf_flag[i] = 1;
    sqf_flag[0] = 0;            /* unused but safe */

    /* Sieve out multiples of each prime square. */
    for (idx_t p = 2; p * p <= n; ++p) {
        idx_t sq = p * p;
        for (idx_t m = sq; m <= n; m += sq) sqf_flag[m] = 0;
    }

    /* Build the prefix sums. */
    sqf_prefix[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sqf_prefix[i] = sqf_prefix[i - 1] + (seq_t)sqf_flag[i];
    }
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;            /* a(1) = 0 by definition */

    /* Ensure that the cache covers 0 .. n-1. */
    if (n >= sqf_cap) recompute_sqf(n);

    /* Count of square‑free numbers strictly less than n. */
    seq_t sqf_before = sqf_prefix[n - 1];

    /* Count square‑free divisors of n that are < n. */
    seq_t bad = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d != n && sqf_flag[d]) ++bad;
            idx_t e = n / d;
            if (e != d && e != n && sqf_flag[e]) ++bad;
        }
    }

    return sqf_before - bad;
}
