/*
 * A271035  Number of 3 X 3 X 3 triangular 0..n arrays with some element less
 *          than a w, nw or ne neighbor exactly once.
 * https://oeis.org/A271035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*binom(n+5,6) + 2*binom(n+4,6) (equivalent to a(n) =
 *            (n^6 + 14n^5 + 75n^4 + 190n^3 + 224n^2 + 96n)/60 )
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 77, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset (the sequence starts at n = 1).  For n = 0 we
       simply return 0; it is never used in the tests but keeps the
       function well‑defined. */
    if (n == 0) return (seq_t)0;

    /* For small seq_t types (≤ 64 bits) we use a 128‑bit intermediate
       to avoid overflow on the intermediate products. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 prod1 = N * (N + 1) * (N + 2) * (N + 3) *
                                  (N + 4) * (N + 5);          /* n*(n+1)*…*(n+5) */
        unsigned __int128 prod2 = N * (N - 1) * (N + 1) * (N + 2) *
                                  (N + 3) * (N + 4);          /* n*(n-1)*…*(n+4) */
        unsigned __int128 res128 = prod1 / 72 + prod2 / 360;   /* 72 = 10*6! , 360 = 2*6! */
        return (seq_t)res128;
    }

    /* For arbitrary‑precision seq_t we rely on its native arithmetic. */
    seq_t N = (seq_t)n;
    seq_t prod1 = N * (N + 1) * (N + 2) * (N + 3) *
                  (N + 4) * (N + 5);          /* n*(n+1)*…*(n+5) */
    seq_t prod2 = N * (N - 1) * (N + 1) * (N + 2) *
                  (N + 3) * (N + 4);          /* n*(n-1)*…*(n+4) */
    seq_t res = prod1 / 72 + prod2 / 360;   /* 72 = 10*6! , 360 = 2*6! */
    return res;
}
