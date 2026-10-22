/*
 * A329550  Total number of consecutive triples of the form (odd, even, odd)
 *          or (even, odd, even) in all permutations of [n].
 * https://oeis.org/A329550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = o*e*(n-2)^2*(n-3)! where * o = ceil(n/2) (number of odds
 *            in [1..n]), * e = floor(n/2) (number of evens in [1..n]). * For
 *            n < 3 the value is 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Handle n < 3: no triples possible */
    if (n < 3) return 0;

    /* o = (n+1)/2 : number of odd elements in [1..n] */
    idx_t o = (n + 1) / 2;
    /* e = n/2 : number of even elements in [1..n]  */
    idx_t e = n / 2;

    /* Convert to seq_t for arithmetic with big integer type */
    seq_t res = (seq_t)1;

    /* o*e */
    res *= (seq_t)o;
    res *= (seq_t)e;

    /* (n-2)^2 */
    seq_t t = (seq_t)(n - 2);
    t *= t;
    res *= t;

    /* (n-3)! */
    seq_t fact = factorial(n - 3);
    res *= fact;

    return res;
}
