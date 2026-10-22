/*
 * A360573  Odd numbers with exactly three zeros in their binary expansion.
 * https://oeis.org/A360573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sequence A360573: odd numbers with exactly three zeros in their binary expansion.
   Computation: Enumerate the positions of the three zeros among the
   lower bits; the numbers are ordered in increasing value. */

/* Helper: binomial coefficient C(n,3) for 0 <= n. */
static inline idx_t binom3(idx_t n)
{
    if (n < 3) return 0ULL;
    /* compute n*(n-1)*(n-2)/6 safely in 64 bit arithmetic */
    return (n * (n - 1) * (n - 2)) / 6ULL;
}

seq_t a(idx_t n)
{
    /* Find the minimal bit length L (>=5) such that the cumulative
       count of terms with at most L bits is at least n. */
    idx_t cum = 0;
    idx_t L = 5;
    while (true) {
        idx_t ways = binom3(L - 2);   /* C(L-2,3) terms of length L */
        if (cum + ways >= n) break;
        cum += ways;
        ++L;
    }

    /* j is the zero‑based index of the desired term within the block of
       length L. */
    idx_t j = n - cum - 1;

    idx_t p = 0, q = 0, r = 0;   /* positions of the three zeros */
    for (int rr = (int)(L - 2); rr >= 3; --rr) {      /* r : highest zero position */
        for (int qq = rr - 1; qq >= 2; --qq) {        /* q : middle zero position */
            for (int pp = qq - 1; pp >= 1; --pp) {    /* p : lowest zero position */
                if (j == 0) {
                    p = pp; q = qq; r = rr;
                    goto found;
                }
                --j;
            }
        }
    }
found:
    /* Construct the number:
       2^L - 1 minus the contributions of the zero bits. */
    seq_t value = ((seq_t)1 << L) - 1U;
    value -= ((seq_t)1 << p);
    value -= ((seq_t)1 << q);
    value -= ((seq_t)1 << r);
    return value;
}
