/*
 * A385682  a(n) = (n - 1) * Sum_{k=2..n} A000010(k).
 * https://oeis.org/A385682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1) * Sum_{k=2..n} phi(k)
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (n - 1) * (A002088(n) - 1) */

#include <stdlib.h>

static seq_t *phi_cache = NULL;     /* phi values, phi[0] unused */
static seq_t *pref_cache = NULL;    /* prefix sums: pref[i] = sum_{k=1..i} phi(k) */
static idx_t  cache_cap   = 0;      /* current maximum index stored */

/* Simple totient calculation by trial division */
static inline seq_t phi_calc(idx_t v)
{
    seq_t result = (seq_t)v;
    idx_t m = v;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / (seq_t)p;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* Extend the caches to cover at least n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;

    idx_t new_cap = (cache_cap ? cache_cap * 2 : 2);
    if (new_cap < n) new_cap = n;

    seq_t *new_phi = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
    seq_t *new_pref = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
    if (!new_phi || !new_pref) exit(1); /* out of memory */

    /* Copy old data */
    for (idx_t i = 0; i <= cache_cap; ++i) {
        new_phi[i] = (cache_cap ? phi_cache[i] : 0);
        new_pref[i] = (cache_cap ? pref_cache[i] : 0);
    }

    /* Compute phi and prefix for new indices */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
        seq_t phi_i = phi_calc(i);
        new_phi[i] = phi_i;
        new_pref[i] = new_pref[i - 1] + phi_i;
    }

    /* Free old arrays if they were allocated */
    if (phi_cache) free(phi_cache);
    if (pref_cache) free(pref_cache);

    phi_cache = new_phi;
    pref_cache = new_pref;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but return 0 */

    ensure_cache(n);

    /* pref_cache[n] = sum_{k=1..n} phi(k)   thus sum_{k=2..n} phi(k) = pref[n] - 1 */
    seq_t sum_phi = pref_cache[n] - (seq_t)1;
    seq_t result = ((seq_t)(n - 1)) * sum_phi;
    return result;
}
