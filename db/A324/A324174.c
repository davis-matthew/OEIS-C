/*
 * A324174  Integers k such that 2*floor(sqrt(k)) divides k.
 * https://oeis.org/A324174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n % 3 == 1: 4*k^2 - 2*k, k = (n + 2)/3 if n % 3 == 2:
 *            4*k^2, k = (n + 1)/3 if n % 3 == 0: 4*k^2 + 4*k, k = n / 3
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)       /* Undefined, but return 0 for safety. */
        return (seq_t)0;

    seq_t n_t = (seq_t)n;
    seq_t k;
    uint64_t r = n % 3;

    if (r == 1) {        /* n = 3*k - 2  */
        k = (n_t + (seq_t)2) / (seq_t)3;
        return (seq_t)4 * k * k - (seq_t)2 * k;
    } else if (r == 2) { /* n = 3*k - 1  */
        k = (n_t + (seq_t)1) / (seq_t)3;
        return (seq_t)4 * k * k;
    } else {             /* n = 3*k      */
        k = n_t / (seq_t)3;
        return (seq_t)4 * k * k + (seq_t)4 * k;
    }
}
