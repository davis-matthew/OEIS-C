/*
 * A264981  Highest power of 9 dividing n.
 * https://oeis.org/A264981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 9^⌊v₃(n)/2⌋ where v₃(n) is the exponent of 3 in n
 * Range:     u8 n <= 728, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute exponent of 3 in n */
    idx_t m = n;
    idx_t v3 = 0;
    while ((m % 3) == 0 && m != 0) {
        m /= 3;
        ++v3;
    }

    /* Compute 9^floor(v3/2) */
    idx_t e = v3 >> 1;          /* floor(v3/2) */
    seq_t result = 1;
    while (e) {
        result = result * (seq_t)9;
        --e;
    }
    return result;
}
