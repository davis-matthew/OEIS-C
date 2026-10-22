/*
 * A381559  Number of minimum dominating sets in the n-triangular honeycomb
 *          bishop graph.
 * https://oeis.org/A381559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%2==0) ? (n/2)! : n * ((n+1)/2)!
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n % 2 == 0) {
        return factorial(n / 2);
    } else {
        seq_t f = factorial((n + 1) / 2);
        return (seq_t)n * f;
    }
}
