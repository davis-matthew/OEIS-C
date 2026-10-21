/*
 * A253179  a(n) = mu(n)*Sum_{k=1..n} (n/k) where mu(n) is Möbius (or Moebius)
 *          function and (x/y) is Kronecker's symbol.
 * https://oeis.org/A253179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(n)*∑_{k=1..n} (n/k)
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   jacobi_idx, kronecker, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n/k) is Kronecker's symbol.  */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/jacobi_idx.h"
#include "../utils/kronecker.h"

/* Main sequence function.  */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int s = kronecker((idx_t)n, k);
        sum += (seq_t)s;
    }
    seq_t mu = mobius(n);
    return mu * sum;
}
