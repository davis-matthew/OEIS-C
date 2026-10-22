/*
 * A369867  a(n) = n * Sum_{p|n, p prime} prime(n/p) / p.
 * https://oeis.org/A369867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * sum_{p|n, p prime} prime(n/p) / p
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_spf, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) -> uint32_t kth prime */
#include "../utils/build_spf.h"   /* build_spf(idx_t limit, idx_t **spf_out) */

static idx_t *spf = NULL;          /* smallest prime factor table */
static idx_t spf_limit = 0;        /* size allocated for spf */

/* Ensure spf table covers at least numbers up to n. */
static void ensure_spf(idx_t n) {
    if (n <= spf_limit) return;
    if (spf) free(spf);
    /* allocate one more to allow indexing by n */
    spf_limit = n + 1;
    build_spf(spf_limit, &spf);
}

/* Compute a(n) according to the OEIS formula. */
seq_t a(idx_t n) {
    if (n == 0) return 0;         /* defined only for n≥1; safe guard */
    /* prepare spf table for factorisation */
    ensure_spf(n);

    seq_t sum = 0;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];                     /* prime divisor */
        idx_t np = n / p;                        /* n divided by p */
        /* term = n * prime(n/p) / p */
        seq_t term = ((seq_t)n) * ((seq_t)get_prime(np)) / ((seq_t)p);
        sum += term;
        while (temp % p == 0) temp /= p;         /* skip same prime */
    }
    return sum;
}
