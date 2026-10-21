/*
 * A259713  a(n) = 3*2^n - 2*(-1)^n.
 * https://oeis.org/A259713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - 2*(-1)^n
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2^n using left shift */
    seq_t two_pow = ((seq_t)1) << (unsigned long)n;

    /* 3*2^n */
    seq_t val = two_pow * (seq_t)3;

    /* Adjust by ±2 depending on parity of n */
    if ((n & 1ULL) == 0) {          /* n even → subtract 2 */
        val -= (seq_t)2;
    } else {                        /* n odd  → add 2   */
        val += (seq_t)2;
    }

    return val;
}
