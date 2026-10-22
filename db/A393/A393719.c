/*
 * A393719  Sum of next n octahedral numbers (A005900).
 * https://oeis.org/A393719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+1)^2*(n^2 + 2)/12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 806,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* offset is 1, but safe */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;                       /* n^2 */
    seq_t f1  = nn;                            /* n */
    seq_t f2  = n2 + 1;                        /* n^2 + 1 */
    seq_t f3  = n2 + 1;                        /* n^2 + 1 again */
    seq_t f4  = n2 + 2;                        /* n^2 + 2 */

    /* we need to divide the product by 12 = 2^2 * 3 */
    int need2 = 2, need3 = 1;
    seq_t *factors[4] = { &f1, &f2, &f3, &f4 };

    for (int i = 0; i < 4; ++i) {
        seq_t *f = factors[i];
        /* burn factors of 2 */
        while (need2 > 0 && (*f % 2) == 0) {
            *f /= 2;
            --need2;
        }
        /* burn a factor of 3 if still needed */
        while (need3 > 0 && (*f % 3) == 0) {
            *f /= 3;
            --need3;
        }
    }

    /* multiply the reduced factors */
    seq_t result = f1;
    result *= f2;
    result *= f3;
    result *= f4;
    return result;
}
