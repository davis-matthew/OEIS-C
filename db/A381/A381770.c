/*
 * A381770  a(n) is the least k > 0 such that the factorial base expansion of
 *          k*n has digits in nonincreasing order.
 * https://oeis.org/A381770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 4235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9155 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A381770:
    a(n) = least k > 0 such that the factorial base expansion of k*n
    has digits in nonincreasing order (read from most significant to
    least significant).  For n = 0 the first term of the sequence is 1.  */

/*  The code uses only integer arithmetic on seq_t and idx_t, and
    works with the arbitrary‑precision build as well as 64‑bit builds.  */

#include <stdbool.h>

/*  Test if the factorial‑base representation of N has
    nonincreasing digits (from most significant to least).  */
static bool factorial_digits_nonincreasing(seq_t N)
{
    seq_t prev = 0;
    bool first = true;
    int i = 1;                      /* factorial position (1! = 1) */
    while (N > 0) {
        seq_t base   = (seq_t)(i + 1);   /* digit i is modulo base */
        seq_t digit  = N % base;
        N          /= base;

        if (!first) {
            /* digits are collected from least significant to most,
               so after the first digit they must be nondecreasing.
               A decrease (digit < prev) means the digits are not
               nonincreasing when read most‑significant first.  */
            if (digit < prev)
                return false;
        }
        prev = digit;
        first = false;
        ++i;
    }
    return true;   /* all digits satisfied (empty representation for 0 is ok) */
}

seq_t a(idx_t n)
{
    /*  Offset 0: a(0) = 1  */
    if (n == 0)
        return 1;

    /*  Convert n to seq_t once for efficiency.  */
    seq_t sn = (seq_t)n;

    /*  Search the minimal positive k.  */
    for (seq_t k = 1;; ++k) {
        if (factorial_digits_nonincreasing(sn * k))
            return k;
    }
}
