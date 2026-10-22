/*
 * A349481  a(n) is the number of Boolean factors of the contranominal scale
 *          of size n by the GreConD algorithm for Boolean matrix
 *          factorization.
 * https://oeis.org/A349481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_2(n)) + ceil(log_2(n))
 * Range:     u8 n <= 513, u16 n <= 513, u32 n <= 513, u64 n <= 513,
 *            u128 n <= 513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(log2(n)) */
    idx_t k = 0;
    idx_t m = n;
    while (m > 1) {
        m >>= 1;
        ++k;
    }

    /* a(n) = 2 * floor(log2(n)) + (n is not a power of two ? 1 : 0) */
    seq_t result = (seq_t)k << 1;
    if ((n & (n - 1)) != 0) {
        ++result;
    }
    return result;
}
