/*
 * A257089  a(n) = log_3 (A256689(n)).
 * https://oeis.org/A257089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over prime‑powers p^e||n of (e + v3(e!)) where v3(e!)
 *            = ⌊e/3⌋ + ⌊e/9⌋ + ⌊e/27⌋ + … (3‑adic valuation of e!) The
 *            sequence is multiplicative and depends only on the prime
 *            exponents of n
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build smallest prime factor table up to a given limit. */
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* defined only for n ≥ 1, guard anyway */

    /* static cache of smallest prime factors */
    static idx_t *spf = NULL;
    static idx_t spf_cap = 0;   /* current maximum limit covered */

    if (n > spf_cap) {
        /* enlarge cache */
        if (spf)
            free(spf);
        build_spf(n, &spf);
        spf_cap = n;
    }

    seq_t total = 0;       /* accumulator, type seq_t for arbitrary precision */

    idx_t t = n;
    while (t > 1) {
        idx_t p = spf[t];          /* smallest prime factor of t */
        idx_t exp = 0;             /* exponent of p in t */
        while (t % p == 0) {
            t /= p;
            exp++;
        }

        /* compute term = exp + v3(exp!) */
        seq_t term = (seq_t)exp;

        /* add v3(exp!) = floor(exp/3) + floor(exp/9) + … */
        idx_t tmp = exp;
        while (tmp >= 3) {
            tmp /= 3;
            term += (seq_t)tmp;
        }

        total += term;
    }

    return total;
}
