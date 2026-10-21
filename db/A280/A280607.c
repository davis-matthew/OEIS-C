/*
 * A280607  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 294", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + (n%2==0 ? 1 : 111) for n>3 a(0) = 1, a(1) = 11,
 *            a(2) = 1, a(3) = 111 (The expression 10^n denotes the decimal
 *            integer 1 followed by n zeros.)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle small indices explicitly */
    switch (n)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)11;
        case 2: return (seq_t)1;
        case 3: return (seq_t)111;
    }

    /* For n > 3 use a(n) = 10^n + 1 if n even, else 10^n + 111 */
    seq_t val = pow10(n);           /* 10^n */
    if ((n & 1) == 0)               /* n even */
        val += (seq_t)1;            /* +1 */
    else
        val += (seq_t)111;          /* +111 */
    return val;
}
