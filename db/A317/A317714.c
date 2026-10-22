/*
 * A317714  Chessboard rectangles sequence (see Comments), also A037270
 *          interleaved with A163102.
 * https://oeis.org/A317714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For n >= 1, a(n) = Sum_{i=1}^{n-1} floor(i/2)^3 which simplifies
 *            to a(n) = (10 - 24*n + 20*n^2 - 8*n^3 + 2*n^4)/64 if n is odd
 *            a(n) = (2*n^4 - 8*n^3 + 8*n^2)/64 if n is even (see OEIS
 *            A317714)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 608, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* behaviour below offset is irrelevant */

    if ((n & 1) == 0) {          /* even n */
        seq_t N   = (seq_t)n;
        seq_t nn  = N * N;       /* n^2 */
        seq_t nn2 = nn * nn;     /* n^4 */
        seq_t num = 2 * nn2 - 8 * N * nn + 8 * nn;
        return num / 64;
    } else {                     /* odd n */
        /* use signed 64‑bit arithmetic; safe for n <= 10⁴ */
        int64_t N   = (int64_t)n;
        int64_t num = 10
                      - 24 * N
                      + 20 * N * N
                      - 8  * N * N * N
                      + 2  * N * N * N * N;
        return (seq_t)num / 64;
    }
}
