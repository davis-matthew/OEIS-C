/*
 * A304578  a(n) = (n^2 + 1) * 5^n + (n^2 + 2) * 3^n.
 * https://oeis.org/A304578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 1) * 5^n + (n^2 + 2) * 3^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;

    seq_t pow5 = pow_small((seq_t)5, n);
    seq_t pow3 = pow_small((seq_t)3, n);

    seq_t term1 = (n2 + 1) * pow5;
    seq_t term2 = (n2 + 2) * pow3;

    return term1 + term2;
}
