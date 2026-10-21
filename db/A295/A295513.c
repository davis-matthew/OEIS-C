/*
 * A295513  a(n) = n*bil(n) - 2^bil(n) where bil(0) = 0 and bil(n) =
 *          floor(log_2(n)) + 1 for n>0.
 * https://oeis.org/A295513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*bil(n) - 2^bil(n) where bil(0) = 0, bil(n) =
 *            floor(log2(n)) + 1 for n > 0
 * Range:     i8 n <= 31, i16 n <= 3071, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0*0 - 2^0 = -1 */
    if (n == 0)
        return (seq_t)-1;

    /* Compute bil(n) = number of bits needed to represent n */
    idx_t bil = 64 - __builtin_clzll(n);   /* works for uint64_t indices */

    /* a(n) = n * bil - 2^bil */
    seq_t result = (seq_t)n * (seq_t)bil - ((seq_t)1 << bil);
    return result;
}
