/*
 * A379732  Decimal expansion of 207/208.
 * https://oeis.org/A379732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10 * ((207 * 10ⁿ) mod 208) / 208⌋ This gives the nth
 *            digit after the decimal point in the exact decimal
 *            representation of 207/208
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants */
    const seq_t num    = (seq_t)207;   /* numerator of 207/208 */
    const seq_t denom  = (seq_t)208;   /* denominator of 207/208 */

    /* compute 10ⁿ (mod denom) using binary exponentiation */
    seq_t pow10 = (seq_t)1;
    seq_t base  = (seq_t)10 % denom;          /* 10 modulo 208 */
    idx_t exp   = n;

    while (exp) {
        if (exp & (idx_t)1)
            pow10 = (pow10 * base) % denom;
        base = (base * base) % denom;
        exp >>= 1;
    }

    /* remainder after n steps: rₙ = (207 * 10ⁿ) mod 208 */
    seq_t rem = (num * pow10) % denom;

    /* nth decimal digit: floor(10 * rem / 208) */
    seq_t digit = (rem * (seq_t)10) / denom;

    return (seq_t)digit;
}
