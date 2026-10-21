/*
 * A271745  Number of set partitions of [n] such that 6 is the largest element
 *          of the last block.
 * https://oeis.org/A271745
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 + 5*2^(n-3) + 35*3^(n-6) + 11*4^(n-6) + 5^(n-6) for
 *            n>6 (with a(6) = 104). The implementation uses binary
 *            exponentiation and works for any seq_t supported by the build
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 6)            /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    if (n == 6)           /* the first term in the OEIS data */
        return (seq_t)104;

    seq_t res = (seq_t)16;
    res += (seq_t)5 * pow_seq((seq_t)2, n - 3);
    res += (seq_t)35 * pow_seq((seq_t)3, n - 6);
    res += (seq_t)11 * pow_seq((seq_t)4, n - 6);
    res += pow_seq((seq_t)5, n - 6);
    return res;
}
