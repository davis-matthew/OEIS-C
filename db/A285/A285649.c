/*
 * A285649  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 73", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=1, a(3)=12; For n>3: a(n)=0 if n is even;
 *            a(n)=2^(n+1)-1 if n is odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_exp(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    static const seq_t init[4] = {(seq_t)1, (seq_t)2, (seq_t)1, (seq_t)12};
    if (n < 4)
        return init[n];
    if ((n & 1) == 0)
        return 0;
    return pow2_exp(n + 1) - 1;
}
