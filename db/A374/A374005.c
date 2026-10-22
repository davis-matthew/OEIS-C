/*
 * A374005  a(n) = (1 + (n+4)^2 + (n-5)*(-1)^n)/2.
 * https://oeis.org/A374005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+4)^2 + (n-5)*(-1)^n)/2
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute numerator N = 1 + (n+4)^2 + (n-5)*(-1)^n */
    seq_t num = (seq_t)1;
    seq_t n_plus_4 = (seq_t)n + (seq_t)4;
    num += n_plus_4 * n_plus_4;         /* square part */
    
    if ((n & (idx_t)1) == 0) {          /* even n */
        if (n >= (idx_t)5) {
            num += (seq_t)(n - (idx_t)5);    /* add (n-5) */
        } else {
            num -= (seq_t)((idx_t)5 - n);    /* subtract (5-n) */
        }
    } else {                            /* odd n */
        if (n >= (idx_t)5) {
            num -= (seq_t)(n - (idx_t)5);    /* subtract (n-5) */
        } else {
            num += (seq_t)((idx_t)5 - n);    /* add (5-n) */
        }
    }
    return num / (seq_t)2;               /* divide by 2 */
}
