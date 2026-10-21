/*
 * A287479  Expansion of g.f. (x + x^2)/(1 + 3*x^2).
 * https://oeis.org/A287479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A287479: Expansion of g.f. (x + x^2)/(1 + 3*x^2)
 * Formula:  a(0) = 0
 *           a(n) = (-3)^((n-1)/2)   for n >= 1
 * (Shows that a(n) satisfies a(n) = -3*a(n-2) for n > 2)
 */

seq_t a(idx_t n)
{
    /* Handle the special first term */
    if (n == 0) {
        return (seq_t)0;
    }

    /* exponent = floor((n-1)/2) */
    idx_t exp = (n - 1) / 2;

    /* Compute (-3)^exp using exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)-3;

    while (exp > 0) {
        if (exp & (idx_t)1U) {
            result = result * base;
        }
        base = base * base;
        exp  = exp >> 1;
    }

    return result;
}
