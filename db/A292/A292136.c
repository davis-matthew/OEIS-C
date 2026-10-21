/*
 * A292136  G.f.: Re(1/(i*x; x)_inf), where (a; q)_inf is the q-Pochhammer
 *          symbol, i = sqrt(-1).
 * https://oeis.org/A292136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 59, i16 n <= 186, i32 n <= 509, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A292136: real part of 1/(i*x; x)_inf,
   computed by dynamic programming over partitions. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* local variables */
    size_t N = (size_t)n;
    size_t size = N + 1;
    seq_t *re, *im;

#if defined(__cplusplus)
    re = new seq_t[size];
    im = new seq_t[size];
#else
    re = calloc(size, sizeof(seq_t));
    im = calloc(size, sizeof(seq_t));
#endif
    if (!re || !im)
        abort();      /* allocation failure */

    /* initialize */
    for (size_t i = 0; i < size; ++i) {
        re[i] = 0;
        im[i] = 0;
    }
    re[0] = 1;
    im[0] = 0;

    /* dynamic programming: multiply by the series
       1 + i*x^k + (i*x^k)^2 + ... for each k. */
    for (size_t k = 1; k <= N; ++k) {
        for (size_t e = k; e <= N; ++e) {
            re[e] += -im[e - k];
            im[e] += re[e - k];
        }
    }

    seq_t result = re[N];

#if defined(__cplusplus)
    delete[] re;
    delete[] im;
#else
    free(re);
    free(im);
#endif
    return result;
}
