/*
 * A359302  Dirichlet g.f.: zeta(s)^2/zeta(3*s-2).
 * https://oeis.org/A359302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 1371, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq A359302
   Multiplicative with the prime power values
   a(p)          = 2
   a(p^e) (e≥2) = 3 - (e-2)·(p²-1)
   (see OEIS for derivation) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* ensure the smallest prime factor sieve contains all numbers up to limit */
static void init_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    if (spf != NULL)
        free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* multiplicative factorization using the spf array */
static seq_t factor_and_compute(idx_t n)
{
    seq_t res = 1;
    while (n > 1) {
        uint64_t p = spf[n];        /* smallest prime divisor */
        idx_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        seq_t val;
        if (e == 1) {
            val = 2;
        } else {
            seq_t p_sq_minus_1 = (seq_t)p * (seq_t)p - 1;
            val = 3 - (seq_t)(e - 2) * p_sq_minus_1;
        }
        res *= val;
    }
    return res;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but harmless */
    init_spf(n);
    return factor_and_compute(n);
}
