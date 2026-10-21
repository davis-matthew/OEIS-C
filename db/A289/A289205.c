/*
 * A289205  a(1) = a(2) = a(3) = 1, a(4) = 3; a(n) = n - a(n-a(n-1)) -
 *          a(n-a(n-2)) for n > 4.
 * https://oeis.org/A289205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=a(3)=1, a(4)=3. For n>4 (k = n/5, r = n%5): a(n) = 1
 *            if r==0 or r==3 a(n) = 5*k - 1 if r==1 a(n) = 2 if r==2 a(n) =
 *            5*k + 1 if r==4
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms explicitly */
    if (n <= 4) {
        static const seq_t init[] = {0, 1, 1, 1, 3};
        return init[n];
    }

    idx_t r = n % 5;     /* remainder modulo 5 */
    idx_t k = n / 5;     /* floor(n/5) */

    if (r == 0 || r == 3) {
        return (seq_t)1;
    } else if (r == 2) {
        return (seq_t)2;
    } else if (r == 1) {
        return (seq_t)(5 * k - 1);
    } else { /* r == 4 */
        return (seq_t)(5 * k + 1);
    }
}
