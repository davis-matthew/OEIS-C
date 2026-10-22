/*
 * A340234  Number of harmonious graphs with n edges and at most n vertices,
 *          allowing self-loops.
 * https://oeis.org/A340234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   if n is odd, a(n) = ceil(n/2)^n; if n is even, a(n) =
 *            ((n^2/4)+(n/2))^(n/2) (conjecture)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 0 for an undefined n = 0, although the sequence is offset 1 */
    if (n == 0)
        return 0;

    /* base of the exponentiation */
    seq_t base;
    /* exponent expressed as idx_t for fast binary exponentiation */
    idx_t exp;

    if (n & 1U) { /* n is odd */
        base = ((seq_t)n + 1) / 2; /* ceil(n/2) */
        exp  = n;
    } else {      /* n is even */
        /* ((n^2)/4) + (n/2)  -- n is even, so all divisions are exact */
        base = (( (seq_t)n * (seq_t)n ) / 4) + ((seq_t)n / 2);
        exp  = n / 2;
    }

    /* binary exponentiation: base^exp in seq_t */
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result;
}
