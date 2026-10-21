/*
 * A255114  Number of length n+7 0..2 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 50 n^4 + 935 n^3 + 73930 n^2 + 248004 n + 137160)
 *            / 120 for n > 5. a(1)=6561, a(2)=14849, a(3)=19338, a(4)=23463,
 *            a(5)=29147
 * Range:     u8 none, u16 n <= 8, u32 n <= 209, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the small indices directly */
    if (n <= 5) {
        static const seq_t val[5] = {6561, 14849, 19338, 23463, 29147};
        return val[n - 1];
    }

    /* Compute the quintic polynomial using 128‑bit integer arithmetic
       to avoid overflow while still returning a value that fits in seq_t. */
    unsigned __int128 x  = (unsigned __int128)n;
    unsigned __int128 n2 = x * x;
    unsigned __int128 n3 = n2 * x;
    unsigned __int128 n4 = n3 * x;
    unsigned __int128 n5 = n4 * x;

    unsigned __int128 N =
        n5 + (unsigned __int128)50   * n4 +
          (unsigned __int128)935   * n3 +
          (unsigned __int128)73930 * n2 +
          (unsigned __int128)248004 * x +
          (unsigned __int128)137160;

    return (seq_t)(N / 120);
}
