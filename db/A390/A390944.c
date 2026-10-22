/*
 * A390944  Value of T[4] after the n-th iteration, when the map T is
 *          initialized with T[1] = 1 and in subsequent iterations, T[v] = k +
 *          (T[v] if defined else 0) for all key-value pairs (k, v) in T; a(n)
 *          = 0 if T[4] isn't defined yet.
 * https://oeis.org/A390944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A390944 - Value of T[4] after the n-th iteration.
   Piecewise definition:
      a(0) = a(1) = a(2) = a(3) = 0
      a(4)  = 1
      a(5)  = 4
      a(6)  = 10
      a(7)  = 12
      a(8)  = 24
      a(9)  = 46
      a(10) = 80
      a(11) = 160
      a(12) = 320
      a(13) = 616
      a(14) = 1222
      a(15) = 2444
      a(16) = 4842
      a(n)  = 2421 * 2^(n-15)   for n > 15   (equivalently 2^(n-16) * a(16) for n >= 16)
*/

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* First 17 values (indices 0..16). */
    static const seq_t init[17] = {
        /* 0 */ (seq_t)0,
        /* 1 */ (seq_t)0,
        /* 2 */ (seq_t)0,
        /* 3 */ (seq_t)0,
        /* 4 */ (seq_t)1,
        /* 5 */ (seq_t)4,
        /* 6 */ (seq_t)10,
        /* 7 */ (seq_t)12,
        /* 8 */ (seq_t)24,
        /* 9 */ (seq_t)46,
        /*10 */ (seq_t)80,
        /*11 */ (seq_t)160,
        /*12 */ (seq_t)320,
        /*13 */ (seq_t)616,
        /*14 */ (seq_t)1222,
        /*15 */ (seq_t)2444,
        /*16 */ (seq_t)4842
    };

    if (n <= 16)
        return init[n];

    /* n > 16: a(n) = 2421 * 2^(n-15) */
    seq_t result = (seq_t)2421;          /* 2421 = a(16) / 2 */
    idx_t i;
    for (i = 0; i < n - 15; ++i)
        result = result + result;        /* double the current value */

    return result;
}
