/*
 * A241436  Number of 2 X n 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it or zero plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A241436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence a(n) = 2*a(n-2) +8*a(n-3) -a(n-4)
 *            -10*a(n-5) -18*a(n-6)
 *            +8*a(n-7)+20*a(n-8)+20*a(n-9)-24*a(n-10)-22*a(n-11)
 *            -a(n-12)+32*a(n-13)+8*a(n-14)-3*a(n-15)-12*a(n-16)
 *            +4*a(n-17)-5*a(n-18)-4*a(n-20)-3*a(n-21)+2*a(n-22) for n > 24.
 *            Initial values are the first 24 terms of the se...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 24 terms */
    static const seq_t init[24] = {
        (seq_t)3ULL,  (seq_t)5ULL,  (seq_t)10ULL, (seq_t)21ULL,
        (seq_t)45ULL, (seq_t)88ULL, (seq_t)181ULL, (seq_t)378ULL,
        (seq_t)710ULL, (seq_t)1460ULL, (seq_t)2973ULL, (seq_t)5668ULL,
        (seq_t)11567ULL, (seq_t)23202ULL, (seq_t)45182ULL, (seq_t)91386ULL,
        (seq_t)182056ULL, (seq_t)358886ULL, (seq_t)721278ULL, (seq_t)1433814ULL,
        (seq_t)2843343ULL, (seq_t)5691577ULL, (seq_t)11312714ULL, (seq_t)22494505ULL
    };

    if (n <= 24) {
        return init[(idx_t)(n - 1)];
    }

    /* allocate array for sequence values */
    seq_t *seq = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    if (!seq) return (seq_t)0; /* allocation failure fallback */

    /* copy initial terms */
    for (idx_t i = 0; i < 24; ++i) {
        seq[i] = init[i];
    }

    /* compute remaining terms */
    for (idx_t k = 24; k < n; ++k) {
        seq_t pos = (seq_t)0;   /* sum of positive contributions */
        seq_t neg = (seq_t)0;   /* sum of negative contributions */

        /* a(k+1) = a(n) with k = n-1 */
        /* add 2*a(k-1) i.e., a(k-2) in 0-based */
        pos += seq[k-2] * (seq_t)2;
        pos += seq[k-3] * (seq_t)8;
        neg += seq[k-4] * (seq_t)1;
        neg += seq[k-5] * (seq_t)10;
        neg += seq[k-6] * (seq_t)18;
        pos += seq[k-7] * (seq_t)8;
        pos += seq[k-8] * (seq_t)20;
        pos += seq[k-9] * (seq_t)20;
        neg += seq[k-10] * (seq_t)24;
        neg += seq[k-11] * (seq_t)22;
        neg += seq[k-12] * (seq_t)1;
        pos += seq[k-13] * (seq_t)32;
        pos += seq[k-14] * (seq_t)8;
        neg += seq[k-15] * (seq_t)3;
        neg += seq[k-16] * (seq_t)12;
        pos += seq[k-17] * (seq_t)4;
        neg += seq[k-18] * (seq_t)5;
        neg += seq[k-20] * (seq_t)4;
        neg += seq[k-21] * (seq_t)3;
        pos += seq[k-22] * (seq_t)2;

        seq[k] = pos - neg;
    }

    seq_t result = seq[n - 1];
    free(seq);
    return result;
}
