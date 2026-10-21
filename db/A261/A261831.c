/*
 * A261831  a(2*n-1) = 2*n-1; otherwise a(n) is the smallest even number not
 *          already present which is obtained from the existing terms by the
 *          rules of (3*n+1)-problem.
 * https://oeis.org/A261831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n when n is odd; otherwise a(n) = { 2*n-2 if n ≡ 0 (mod
 *            6) 2*n if n ≡ 2 (mod 6) n-2 if n ≡ 4 (mod 6) }
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Every odd index maps to the odd number itself. */
    if (n & 1ULL)
        return (seq_t)n;

    seq_t x = (seq_t)n;
    idx_t m = n % 6U;        /* m is 0,2,4 for even n */

    switch (m) {
        case 0:                 /* n ≡ 0 (mod 6) */
            return x + x - (seq_t)2;
        case 2:                 /* n ≡ 2 (mod 6) */
            return x + x;
        case 4:                 /* n ≡ 4 (mod 6) */
            return x - (seq_t)2;
        default:                /* Should never be reached */
            return (seq_t)0;
    }
}
