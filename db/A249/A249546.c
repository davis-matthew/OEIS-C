/*
 * A249546  Speed of light in km/h.
 * https://oeis.org/A249546
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = floor((1079252848*10 + 8) / 10^(20-n)) % 10 for 10 ≤ n ≤
 *            20, a(n) = 0 otherwise. This returns successive decimal digits
 *            of the constant 1079252848.8 km/h, starting with the most
 *            significant digit at n = 10
 * Range:     u8 none, u16 none, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return 0;                  /* values below the offset are irrelevant */

    idx_t rel = n - 10;             /* relative position (0 for the first digit) */

    static const seq_t int_part = 1079252848;   /* integer part of the constant */
    static const seq_t frac_digit = 8;          /* single fractional digit */

    /* powers of ten up to 10^9 (fits into 32 bits) */
    static const seq_t pow10[10] = {
        1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
        100000ULL, 1000000ULL, 10000000ULL,
        100000000ULL, 1000000000ULL
    };

    if (rel < 10) {
        /* digit from the integer part:
           rel = 0 -> most significant digit
           rel = 9 -> least significant integer digit */
        idx_t exp = 9 - rel;            /* exponent of 10 */
        return (int_part / pow10[exp]) % 10;
    }

    if (rel == 10)        return frac_digit;   /* the sole fractional digit */

    /* n > 20: all remaining digits are 0 because the decimal expansion terminates */
    return 0;
}
