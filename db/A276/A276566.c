/*
 * A276566  Decimal expansion of 466/885.
 * https://oeis.org/A276566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 466/885.
   a(n) is the n-th digit after the decimal point of the fraction 466/885. */
seq_t a(idx_t n)
{
    /* constants p/q = 466/885 */
    const unsigned long long p = 466ULL;
    const unsigned long long q = 885ULL;

    unsigned long long r = p % q;          /* initial remainder */
    unsigned long long digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10ULL;                        /* bring down next 0 */
        digit = r / q;                     /* next decimal digit */
        r     = r % q;                     /* new remainder */
        if (i == n)
            return (seq_t)digit;           /* return the desired digit */
    }

    /* should never reach here */
    return 0;
}
