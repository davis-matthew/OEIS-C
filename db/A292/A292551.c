/*
 * A292551  Expansion of x*(1 - 2*x + x^2 + 7*x^3 - x^4)/((1 - x)^4*(1 +
 *          x)^3).
 * https://oeis.org/A292551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2*n^3 - 2*n^2 + (-1)^n*(2*n^2 - 11*n - 6) - 5*n + 6)/16
 * Range:     i8 n <= 4, i16 n <= 25, i32 n <= 1190, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the explicit closed‑form for A292551. */
seq_t a(idx_t n)
{
    /* cast n to seq_t once to avoid repeated conversion */
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;       /* n^2   */
    seq_t nn3 = nn2 * nn;      /* n^3   */

    /* parity part: (-1)^n * (2*n^2 - 11*n - 6) */
    seq_t parity_term = 2 * nn2 - 11 * nn - 6;
    if (n & 1)                  /* odd n: multiply by -1 */
        parity_term = -parity_term;

    /* full numerator */
    seq_t numerator = 2 * nn3 - 2 * nn2 + parity_term - 5 * nn + 6;

    /* division by 16 (exact for all integer n >= 0) */
    return numerator / 16;
}
