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
 * Validated: standard, pass, 33 terms through n = 334 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper to compute the polynomial part of the formula */
static seq_t poly_of(idx_t n) {
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;              /* n^2 */
    seq_t n3 = n2 * n1;              /* n^3 */
    seq_t n4 = n3 * n1;              /* n^4 */
    seq_t n5 = n4 * n1;              /* n^5 */
    seq_t n6 = n5 * n1;              /* n^6 */
    seq_t n7 = n6 * n1;              /* n^7 */
    seq_t n8 = n7 * n1;              /* n^8 */

    seq_t res = (seq_t)8 * n8;
    res -= (seq_t)140 * n7;
    res += (seq_t)1010 * n6;
    res -= (seq_t)3913 * n5;
    res += (seq_t)9201 * n4;
    res -= (seq_t)15662 * n3;
    res += (seq_t)34500 * n2;
    res -= (seq_t)120552 * n1;
    res += (seq_t)221760;
    return res;
}

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
    seq_t pow2 = ipow((seq_t)2, (seq_t)(n - 7));
    seq_t pown = ipow((seq_t)n, (seq_t)(n - 9));
    seq_t poly = poly_of(n);

    seq_t mult = pow2;
    mult *= pown;
    mult *= (seq_t)(n - 4);
    mult *= poly;

    return mult / 6;
}
