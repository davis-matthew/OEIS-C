/*
 * A255051  a(1)=1, a(n+1) = a(n)/gcd(a(n),n) if this GCD is > 1, else a(n+1)
 *          = a(n) + n + 1.
 * https://oeis.org/A255051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(4k)=2; a(4k+1)=1; a(4k+2)=4k+3; a(4k+3)=8k+6
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned, so the low two bits give n mod 4 */
    idx_t r = n & 3;
    switch (r) {
        case 1:  /* n ≡ 1 (mod 4) */
            return (seq_t)1;

        case 2:  /* n ≡ 2 (mod 4) */
            return (seq_t)n + 1;

        case 3:  /* n ≡ 3 (mod 4) */
            return ((seq_t)2 * ((seq_t)n + 1)) - (seq_t)2;

        case 0:  /* n ≡ 0 (mod 4) */
            return (seq_t)2;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
