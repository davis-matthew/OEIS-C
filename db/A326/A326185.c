/*
 * A326185  a(n) = sigma(n) - A057521(n) - n.
 * https://oeis.org/A326185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 2338, i32 n <= 9704, i64 n <= 9704,
 *            i128 n <= 9704, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sigma(n) - A057521(n) - n
 * where A057521(n) = the product of all prime powers p^e with e ≥ 2
 * (i.e., the square‑ful part of n; for squarefree n, A057521(n) = 1).
 * The function uses a smallest‑prime‑factor sieve and updates
 * a static cache so recomputations are fast. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static idx_t *spf = NULL;    /* smallest prime factor array */
static idx_t spf_limit = 0;  /* size that has been built */

/* ------------------------------------------------------------------ */
/*  sigma(n)  and  A057521(n)  -------------------------------------- */

static void
ensure_spf(idx_t n)
{
    if (n > spf_limit) {
        if (spf) free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }
}

/* ------------------------------------------------------------------ */
/*  public function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) /* under‑offset; behaviour undefined, return 0 */
        return 0;

    ensure_spf(n);

    seq_t sigma   = 1;          /* product of sigma factors */
    seq_t prodPS  = 1;          /* product of all p^e with e ≥ 2 */

    idx_t m = n;
    while (m > 1) {
        idx_t p   = spf[m];
        idx_t exp = 0;
        seq_t power = 1;          /* p^exp */

        /* factor fully */
        while (m % p == 0) {
            m   /= p;
            exp++;
            power *= (seq_t)p;
        }

        /* sigma contribution: (p^(exp+1)-1)/(p-1) */
        seq_t numerator   = power * (seq_t)p - 1; /* p^(exp+1)-1 */
        seq_t denom       = (seq_t)(p - 1);
        seq_t sigma_factor = numerator / denom;
        sigma *= sigma_factor;

        /* A057521 accumulation */
        if (exp > 1)
            prodPS *= power;
    }

    seq_t result = sigma - prodPS - (seq_t)n;
    return result;
}
