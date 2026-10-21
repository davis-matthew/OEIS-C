/*
 * A244498  Number of magic labelings of the nodes of the 4 X 4 grid graph
 *          with magic sum n.
 * https://oeis.org/A244498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 22, u64 n <= 283, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=0}^6 c_k * binom(n+9-k,9)
 *   with coefficients c = [1,26,131,212,131,26,1]
 * This is equivalent to a(n) = (7560 + 34164*n + 67044*n^2 + 75190*n^3 +
 *                             53382*n^4 + 25095*n^5 + 7896*n^6 + 1620*n^7 +
 *                             198*n^8 + 11*n^9) / 7560.
 */
#include "../utils/binom.h"

seq_t a(idx_t n) {
    static const int coeff[7] = {1, 26, 131, 212, 131, 26, 1};
    seq_t result = 0;
    for (idx_t k = 0; k < 7; ++k) {
        idx_t N = n + 9 - k;          /* compute n + 9 - k   */
        seq_t term = binom(N, 9);     /* binom(N,9) uses seq_t arithmetic */
        term = term * (seq_t)coeff[k];
        result = result + term;
    }
    return result;
}
