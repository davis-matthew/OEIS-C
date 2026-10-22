/*
 * A344853  a(n) = n minus (sum of digits of n in base 3).
 * https://oeis.org/A344853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n minus (sum of digits of n in base 3)
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute sum of ternary digits of n */
    idx_t t = n;
    seq_t sum = 0;
    while (t != 0) {
        sum += (seq_t)(t % 3);
        t /= 3;
    }
    /* Result = n - sum */
    return (seq_t)n - sum;
}
