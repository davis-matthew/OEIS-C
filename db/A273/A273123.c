/*
 * A273123  Values of A007692(n) that are not of the form x^2 + y^2 + z^2
 *          where x, y, z are nonzero integers.
 * https://oeis.org/A273123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 85*4^((n-1)/2) if n is odd, * a(n) = 130*4^(n/2-1) if n
 *            is even
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No term defined for n = 0; return 0 to avoid UB on such calls. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t base;
    idx_t mult;          /* exponent of 4 */

    if (n & 1ULL) {      /* odd n */
        base = (seq_t)85;
        mult = (n - 1) / 2;
    } else {             /* even n */
        base = (seq_t)130;
        mult = (n / 2) - 1;
    }

    seq_t pow4 = (seq_t)1;
    while (mult > 0) {
        pow4 *= (seq_t)4;
        --mult;
    }

    return base * pow4;
}
