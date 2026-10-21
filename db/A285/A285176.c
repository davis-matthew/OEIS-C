/*
 * A285176  Elastic numbers: numbers n with two or more digits such that for
 *          any number m made by inserting any number of 0's between the
 *          digits of n, m is a multiple of n.
 * https://oeis.org/A285176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1..12) are the 12 elastic numbers with a single trailing zero
 *            or two non‑zero digits (10,15,18,20,30,40,45,50,60,70,80,90).
 *            For n ≥ 13 the elastic numbers are all numbers of the form d ·
 *            10^e (d = 1..9, e ≥ 2) sorted in ascending order. Hence, for n ≥
 *            13: let r = n – 12, e = 2 + (r‑1)/9, // exp...
 * Range:     u8 n <= 14, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static const seq_t elastic_first12[12] = {
    10, 15, 18, 20, 30, 40, 45, 50, 60, 70, 80, 90
};

seq_t a(idx_t n)
{
    if (n <= 12)
        return elastic_first12[n - 1];

    idx_t r = n - 12;                 // 1‑based offset inside the blocks of e≥2
    idx_t e = 2 + (r - 1) / 9;        // exponent of ten
    idx_t d = (r - 1) % 9 + 1;        // leading digit (1..9)

    /* d is small (≤9) so cast safely to seq_t, then multiply */
    return (seq_t)d * pow10(e);
}
