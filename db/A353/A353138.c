/*
 * A353138  Sum of (the number of digits in n to the power (each digit in n)).
 * https://oeis.org/A353138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = Sum for each decimal digit d of n: (num_digits(n))^d
 * Range:     u8 n <= 17, u16 n <= 1007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where num_digits(n) is the count of decimal digits in n. */

seq_t a(idx_t n)
{
    /* compute number of decimal digits of n */
    idx_t tmp = n;
    int d = 0;
    while (tmp > 0) {
        ++d;
        tmp /= 10;
    }

    /* sum d^digit for each digit of n */
    seq_t sum = 0;
    tmp = n;
    while (tmp > 0) {
        int dig = (int)(tmp % 10);   /* digit in 0..9 */
        seq_t p = 1;
        seq_t dseq = (seq_t)d;
        for (int i = 0; i < dig; ++i) {
            p *= dseq;
        }
        sum += p;
        tmp /= 10;
    }

    return sum;
}
