/*
 * A330179  a(n) = n + floor(ns/r) + floor(nt/r), where r = e - 1, s = e, t =
 *          e + 1.
 * https://oeis.org/A330179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*e/(e-1)) + floor(n*(e+1)/(e-1))
 * Range:     u8 none, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_convergent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/get_convergent.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    static bool init = false;
    static unsigned long long pa = 0, qa = 0;          /* e/(e-1) */
    static unsigned long long pb = 0, qb = 0;          /* (e+1)/(e-1) */

    if (!init) {
        init = true;

        /* Compute approximations for α = e/(e-1) and β = (e+1)/(e-1) */
        long double e = expl(1.0L);                    /* long double e */
        get_convergent(e / (e - 1.0L), &pa, &qa);      /* α */
        get_convergent((e + 1.0L) / (e - 1.0L), &pb, &qb);  /* β' */
    }

    seq_t n_seq = (seq_t)n;
    seq_t term1 = (n_seq * (seq_t)pa) / (seq_t)qa;
    seq_t term2 = (n_seq * (seq_t)pb) / (seq_t)qb;

    return n_seq + term1 + term2;
}
