/*
 * A398083  a(n) = 2^n mod 73.
 * https://oeis.org/A398083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 73
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binary exponentiation: keep intermediate values < 73 */
    idx_t res   = 1;          /* result (res = 2^k mod 73) */
    idx_t base  = 2 % 73;     /* current power of two (2^(2^i) mod 73) */
    idx_t exp   = n;          /* remaining exponent */

    while (exp > 0) {
        if (exp & 1)          /* if lowest bit of exp is set */
            res = (res * base) % 73;
        base = (base * base) % 73; /* square the base */
        exp >>= 1;           /* shift exponent right by one bit */
    }

    return (seq_t)res;        /* cast back to the sequence type */
}
