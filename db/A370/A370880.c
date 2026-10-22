/*
 * A370880  Natural numbers repeated 3 times are taken in parts of successive
 *          lengths 1,2,3,..., and a(n) is the sum of the numbers in the part
 *          with length n.
 * https://oeis.org/A370880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+3)/6 if n mod 3 = 0 (n*(n^2+3)+2)/6 if n mod 3 = 1
 *            (n*(n^2+3)-2)/6 if n mod 3 = 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {                  /* offset is 1; 0 not used, return 0 for safety */
        return (seq_t)0;
    }

    idx_t rem = n % 3U;            /* remainder 0,1,2 */

    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;   /* n^2 */
    seq_t base  = n_sq + (seq_t)3; /* n^2 + 3 */

    seq_t numerator = n_seq * base;        /* n*(n^2+3) */

    if (rem == 0U) {
        return numerator / (seq_t)6;
    } else if (rem == 1U) {
        return (numerator + (seq_t)2) / (seq_t)6;
    } else { /* rem == 2U */
        return (numerator - (seq_t)2) / (seq_t)6;
    }
}
