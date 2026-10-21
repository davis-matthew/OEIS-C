/*
 * A277927  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 5", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A277927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1. For n>1 - if n is even, a(n)=0 - if n is odd,
 *            a(n) is the repunit consisting of (n+1) ones (e.g. n=3 → 1111)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small fixed values */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Even indices greater than 1 produce zero */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* Odd indices >=3: construct the repunit of length n+1.
       Starting with a single 1, repeat n times: rep = rep*10 + 1.
       The loop runs exactly n iterations, yielding length 1+n. */
    seq_t rep = 1;
    for (idx_t i = 0; i < n; ++i)
        rep = rep * 10 + 1;

    return rep;
}
