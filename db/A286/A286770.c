/*
 * A286770  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 221", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 10, a(2) = 0, a(n) = 1 for n > 2 even, a(n) =
 *            10*(10^n - 1)/9 for n > 2 odd (i.e. a(n) is a decimal number
 *            consisting of n ones followed by 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  // for bool, not used but allowed

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)10;
    }
    if (n == 2) {
        return (seq_t)0;
    }

    /* even indices > 2 */
    if ((n & 1) == 0) {
        return (seq_t)1;
    }

    /* odd indices > 2 */
    seq_t rep = 0;                /* will hold the repunit of length n */
    for (idx_t i = 0; i < n; ++i) {
        rep = rep * (seq_t)10 + (seq_t)1;  /* compute 111...1 */
    }
    return rep * (seq_t)10;       /* append trailing 0 */
}
