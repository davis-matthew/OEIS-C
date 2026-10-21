/*
 * A248160  Expansion of (1 - 2*x^2)/(1 + x)^5. Fourth column of Riordan
 *          triangle A248156.
 * https://oeis.org/A248160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*(n+1)*(n+2)*(12 + 9*n - n*n)/24
 * Range:     i8 n <= 1, i16 n <= 16, i32 n <= 219, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the unsigned index to seq_t once */
    seq_t nn = (seq_t)n;

    /* Compute the three factors of the formula */
    seq_t f1 = nn + (seq_t)1;
    seq_t f2 = nn + (seq_t)2;
    seq_t f3 = (seq_t)12 + (seq_t)9 * nn - nn * nn;

    /* Multiply the factors together */
    seq_t prod = f1 * f2 * f3;

    /* Divide by 24 (4! = 24) */
    seq_t res = prod / (seq_t)24;

    /* Apply the (-1)^n factor */
    if (n & (idx_t)1)
        res = -res;

    return res;
}
