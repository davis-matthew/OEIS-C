/*
 * A249115  Floor(r*n), where r = (5 - sqrt(5))/2; the Beatty complement of
 *          A003231.
 * https://oeis.org/A249115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((5*n) - floor(n*sqrt(5))) / 2), with a special
 *            adjustment: if the numerator is even, subtract 1 after dividing
 *            by 2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;                     /* convert index to seq_t */
    seq_t n_sq   = n_seq * n_seq;               /* n^2 */
    seq_t five_n_sq = (seq_t)5 * n_sq;          /* 5*n^2 */
    seq_t sqrt_part = floor_sqrt(five_n_sq);    /* floor(n*sqrt5) */
    seq_t five_n = (seq_t)5 * n_seq;            /* 5*n */
    seq_t numer  = five_n - sqrt_part;          /* numerator */

    /* numer is always positive; compute floor(numer/2), subtract 1 if even */
    if ((numer & (seq_t)1) == 0) {
        /* even */
        return (numer >> 1) - (seq_t)1;
    } else {
        /* odd */
        return numer >> 1;
    }
}
