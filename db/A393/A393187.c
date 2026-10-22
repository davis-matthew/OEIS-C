/*
 * A393187  a(n) = n XOR (2 * (n AND -n)) assuming a two's complement
 *          representation of negative values.
 * https://oeis.org/A393187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n XOR (2 * (n AND -n))
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t lowbit = x & (~x + (seq_t)1);   /* lowbit = n & -n */
    seq_t dbl     = lowbit * (seq_t)2;    /* 2 * (n AND -n) */
    return x ^ dbl;                      /* n XOR (2 * (n AND -n)) */
}
