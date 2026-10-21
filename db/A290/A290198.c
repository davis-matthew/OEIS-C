/*
 * A290198  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 721", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2n+1) for n <= 3, a(n) = 2^(n+1) - 2 for n >= 4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 3) {
        return fib(2 * n + 1);
    } else {
        return pow2(n + 1) - (seq_t)2;
    }
}
