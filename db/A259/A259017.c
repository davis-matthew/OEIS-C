/*
 * A259017  Number of fixed tree polycubes of size n that are proper in n-4
 *          dimensions.
 * https://oeis.org/A259017
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-7)*n^(n-9)*(n-4)*(8*n^8 - 140*n^7 + 1010*n^6 -
 *            3913*n^5 + 9201*n^4 - 15662*n^3 + 34500*n^2 - 120552*n +
 *            221760)/6
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, poly_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/poly_of.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 4) return 0;

    /* specific for n = 4…8 where the formula would involve negative exponents */
    static const seq_t small_values[] = {
        0ull,          /* a(4) */
        1ull,          /* a(5) */
        172ull,        /* a(6) */
        17041ull,      /* a(7) */
        1382400ull,    /* a(8) */
    };
    if (n <= 8) return small_values[n - 4];

    /* For n >= 9 use the closed formula */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 7));
    seq_t pown = big_pow((seq_t)n, (seq_t)(n - 9));
    seq_t poly = poly_of(n);

    seq_t mult = pow2;
    mult *= pown;
    mult *= (seq_t)(n - 4);
    mult *= poly;

    return mult / 6;
}
