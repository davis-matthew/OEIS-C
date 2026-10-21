/*
 * A296775  Expansion of 1/Sum_{k>=0} A000326(k+1)*x^k.
 * https://oeis.org/A296775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = -5, a(2) = 13.
 * For n >= 3, a(n) = (-1)^n * 27 * 2^(n-3).
 * Recurrence: a(n) = -2*a(n-1) for n > 3.
 *
 * The computation uses only integer arithmetic on seq_t and idx_t.
 * It works for arbitrary-precision seq_t as well as fixed-size
 * signed integer types (int8_t … __int128).
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-5;
    if (n == 2) return (seq_t)13;

    /* Compute 27 * 2^(n-3) */
    seq_t value = (seq_t)27;
    idx_t exp = n - 3;
    while (exp > 0) {
        value = value * (seq_t)2;
        exp--;
    }

    /* Apply the sign (-1)^n */
    if (n & 1) {
        value = -value;
    }

    return value;
}
