/*
 * A271989  g_n(8) where g is the weak Goodstein function defined in A266202.
 * https://oeis.org/A271989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = b_{n+1}(a(n-1)) - 1, where a(0) = 8 and b_k(x)
 *            reinterprets the base-k representation of x as a number in base
 *            (k+1)
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   reinterpret_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for basic integer types */
#include "../utils/reinterpret_base.h"

/* public sequence function */
seq_t a(idx_t n)
{
    seq_t val = 8;                 /* a(0) = 8 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t new_val = reinterpret_base(val, i + 1);
        val = new_val - 1;
    }
    return val;
}
