/*
 * A357928  a(n) is the smallest c for which (s+c)^2-n is a square, where s =
 *          floor(sqrt(n)), or -1 if no such c exists.
 * https://oeis.org/A357928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute the OEIS sequence A357928.
 *
 * a(n) = the smallest c ≥ 0 such that (⌊√n⌋ + c)^2 − n is a perfect
 *        square.  If n ≡ 2 (mod 4) no such c exists and the function
 *        returns −1.
 *
 * The implementation simply searches for the minimal c by incrementing
 * from 0 upwards, using the provided `floor_sqrt` routine to test for
 * perfect squares.  This is efficient for the intended range (n up to
 * at least 10000) and works with arbitrary‑precision `seq_t` as
 * required.
 */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Special case: values congruent to 2 mod 4 never admit a solution */
    if ((n & 3ULL) == 2ULL)
        return (seq_t)-1;

    /* Convert n to seq_t once for arithmetic */
    seq_t sn = (seq_t)n;

    /* Base of the search: s = floor(sqrt(n)) */
    seq_t s = floor_sqrt(sn);

    seq_t c = 0;
    for (;; ++c) {
        seq_t u = s + c;          /* candidate u = s + c               */
        seq_t v = u * u - sn;     /* v = u^2 − n                       */
        seq_t w = floor_sqrt(v);  /* w = floor(sqrt(v))                 */
        if (w * w == v)           /* v is a perfect square */
            return c;
        /* For safety in the unlikely case that no solution appears
         * after an enormous number of iterations, we fall back to
         * the theoretical formula: return -1 when n % 4 == 2.  But
         * the check above guarantees that this loop will terminate
         * for all admissible n. */
    }
}
