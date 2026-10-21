/*
 * A289742  Sizes of wrenches (or spanners) using imperial units.
 * https://oeis.org/A289742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, dumb, fini
 * Formula:   a(n) = numerator if n is odd, else denominator of reduced
 *            fraction (k)/16 where k = 4 + floor((n-1)/2). The fraction k/16
 *            is reduced by dividing numerator and denominator by gcd(k,16).
 *            The sequence lists numerators and denominators alternately
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned int _gcd16[16] = {
    16,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1
};

seq_t a(idx_t n) {
    /* indices start at 1; for safety return 0 for n<1 */
    if (n < 1) return 0;

    /* pair index (0 for first pair k=4, 1 for second pair k=5, ...) */
    idx_t pair = (n - 1) >> 1;
    /* integer part of fraction: k/16 */
    idx_t k = 4 + pair;
    /* get gcd(k,16) using only the low 4 bits of k */
    unsigned int g = _gcd16[(unsigned int)(k & 15)];
    /* denominator = 16 / gcd */
    seq_t denom = (seq_t)16 / g;
    /* numerator = k / gcd */
    seq_t numer = (seq_t)k / g;
    /* alternately return numerator and denominator */
    return (n & 1) ? numer : denom;
}
