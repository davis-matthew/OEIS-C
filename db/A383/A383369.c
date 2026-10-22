/*
 * A383369  Population of elementary triangular automaton rule 90 at
 *          generation n, starting from a lone 1 cell at generation 0.
 * https://oeis.org/A383369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 For n ≥ 2: if n is even (n = 2k): if k is
 *            even: a(n) = a(k) else if k mod 4 == 1: a(n) = 3 * a(k) / 2
 *            else: a(n) = 2 * a(k) if n is odd (n = 2k + 1): a(n) = (k is
 *            even ? 4 : 2) * a(n – 1)
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 9498, u64 n <= 9498,
 *            u128 n <= 9498, big any n
 * Helpers:   recurse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/recurse.h"

seq_t a(idx_t n)
{
    return recurse(n);
}
