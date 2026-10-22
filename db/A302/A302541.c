/*
 * A302541  The simple continued fraction for 1/0.45359237 (approximately
 *          2.20462262), which is the number of pounds per kilogram.
 * https://oeis.org/A302541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, fini, full, nonn
 * Formula:   a(n) = n-th quotient in the simple continued fraction of
 *            100000000/45359237 (the pounds per kilogram ratio). The sequence
 *            terminates after the last quotient (return 0 for indices beyond
 *            the length)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The continued fraction is obtained from the Euclidean algorithm on */
    /* 100000000 / 45359237.  The safe working type is 64‑bit unsigned. */
    static const unsigned long long NUM = 100000000ULL;
    static const unsigned long long DEN = 45359237ULL;

    unsigned long long p = NUM, q = DEN;

    for (idx_t i = 0; i <= n; ++i) {
        if (q == 0)          /* reached the end of the continued fraction */
            return 0;

        unsigned long long term = p / q;   /* current quotient */

        if (i == n)          /* desired term reached */
            return (seq_t)term;

        unsigned long long r = p % q;  /* remainder for next step */
        p = q;
        q = r;
    }

    return 0;   /* safety net – should never be reached */
}
