/*
 * A304509  a(n) = 63*2^n - 39 (n>=1).
 * https://oeis.org/A304509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*2^n - 39, n>=1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; the sequence is defined for
     * n >= 1.  No value is required for n < 1. */
    if (n < 1)
        return (seq_t)0;

    /* Fast binary exponentiation for 2^n.  All arithmetic is performed on
     * seq_t, which handles unsigned overflow correctly whether it is a
     * fixed‑width integer or an arbitrary‑precision type. */
    seq_t result = (seq_t)1;          /* 2^0 */
    seq_t base   = (seq_t)2;          /* base 2 */
    idx_t exp    = n;                 /* exponent */

    while (exp > 0) {
        if (exp & (idx_t)1)          /* if lowest bit is set, multiply result */
            result *= base;
        base *= base;                /* square the base */
        exp >>= 1;                   /* shift right */
    }

    /* Compute 63*2^n - 39 */
    result *= (seq_t)63;
    result -= (seq_t)39;
    return result;
}
