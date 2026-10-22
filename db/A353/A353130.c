/*
 * A353130  a(n) = floor(H(n) + exp(H(n))*log(H(n))) - n, where H(n) =
 *          Sum_{k=1..n} 1/k.
 * https://oeis.org/A353130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( H(n) + exp(H(n))*log(H(n)) ) – n * where H(n) =
 *            Sum_{k=1..n} 1/k * The function caches harmonic sums in a
 *            long‑double array for incremental * O(n) construction and uses
 *            the long‑double arithmetic directly, * converting the resulting
 *            integer to seq_t
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Cache for harmonic numbers H(0) … H(max_n). */
static long double *H_cache = NULL;
static unsigned long long H_max  = 0;   /* largest n cached */

/* Extend the harmonic cache to include H(n). */
static void ensure_harmonic(unsigned long long n)
{
    if (n <= H_max)
        return;

    /* Allocate or expand the array. */
    unsigned long long new_cap = n + 1;
    long double *tmp = (long double *)realloc(H_cache, new_cap * sizeof(long double));
    if (!tmp)  /* out of memory – give up */
        return;
    H_cache = tmp;
    if (H_max == 0)
        H_cache[0] = 0.0L;          /* H(0) = 0 */

    /* Compute missing terms. */
    long double sum = H_cache[H_max];
    for (unsigned long long i = H_max + 1; i <= n; ++i) {
        sum += 1.0L / (long double)i;
        H_cache[i] = sum;
    }
    H_max = n;
}

/* Sequence element. */
seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;

    /* Ensure H(nn) is available. */
    ensure_harmonic(nn);

    /* Compute H(n) and the real expression. */
    long double H   = H_cache[nn];
    long double val = H + expl(H) * logl(H);

    /* Floor the value; use a tiny epsilon to guard against round‑off. */
    unsigned long long floor_val = (unsigned long long)(val + 1e-12L);

    /* Convert to seq_t and subtract n. */
    return (seq_t)floor_val - (seq_t)n;
}
