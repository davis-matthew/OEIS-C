/*
 * A366918  a(n) is the number of multiples of n that have exactly n divisors,
 *          or -1 if there are infinitely many such numbers.
 * https://oeis.org/A366918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = omega(n)! when n is squarefree, * 1 when n = 4, * -1
 *            otherwise * (omega(n) is the number of distinct prime factors of
 *            n)
 * Range:     i8 n <= 2231, i16 n <= 9730, i32 n <= 9730, i64 n <= 9730,
 *            i128 n <= 9730, big any n
 * Helpers:   bigomega, factorial, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/bigomega.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)4)
        return (seq_t)1;

    if (!is_squarefree((seq_t)n))
        return (seq_t)-1;

    seq_t w = bigomega(n);  /* number of distinct prime factors */
    return factorial((idx_t)w);
}
