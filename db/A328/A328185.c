/*
 * A328185  Numerators associated with A328184.
 * https://oeis.org/A328185
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator((n-1)/(2*n)) for even n; a(n) =
 *            numerator((2*n-3)/(4*n)) for odd n. Offset: 3
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values only defined for n >= 3; for smaller n return 0 */
    if (n < 3)
        return 0;

    /* even n: numerator is n-1 (gcd(n-1,2n)=1) */
    if ((n & 1ULL) == 0) {
        return (seq_t)n - 1;
    }

    /* odd n */
    /* gcd(2*n-3, 4*n) is 3 iff n is a multiple of 3, otherwise 1 */
    if ((n % 3ULL) == 0) {
        /* (2*n-3)/3 */
        return ((seq_t)2 * n - 3) / 3;
    } else {
        /* 2*n-3 */
        return ((seq_t)2 * n) - 3;
    }
}
