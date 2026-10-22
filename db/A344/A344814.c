/*
 * A344814  a(n) = Sum_{k=1..n} floor(n/k) * 3^(k-1).
 * https://oeis.org/A344814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A344814
 * Formula: a(n) = Sum_{k=1..n} floor(n/k) * 3^(k-1)
 * Offset: 1  (a(1) is the first term)
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow3 = 1;  /* 3^(k-1) for current k */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t f = (seq_t)(n / k);   /* floor(n/k) */
        sum += f * pow3;
        pow3 *= 3;
    }

    return sum;
}
