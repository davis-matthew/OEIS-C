/*
 * A300999  Add to a(n) the first digit of a(n+1) to get a(n+2), with a(1) = 1
 *          and a(2) = 2.
 * https://oeis.org/A300999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=2; a(n+2) = a(n) + first_digit(a(n+1)), where
 *            first_digit(x) is the most significant decimal digit of x
 * Range:     u8 n <= 294, u16 n <= 8820, u32 n <= 8820, u64 n <= 8820,
 *            u128 n <= 8820, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>              /* for uint64_t if needed */

/* local helper – compute most significant decimal digit */
static seq_t first_digit(seq_t x)
{
    /* repeatedly divide by 10 until only one digit remains */
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return x;
}

/* Main function for OEIS sequence A300999 */
seq_t a(idx_t n)
{
    /* Handle the base cases explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a1 = (seq_t)1;   /* a(1)   */
    seq_t a2 = (seq_t)2;   /* a(2)   */
    seq_t an;              /* current term a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t fd = first_digit(a2);   /* first digit of a(i-1) */
        an = a1 + fd;                 /* a(i) = a(i-2) + fd(a(i-1)) */
        a1 = a2;                      /* shift for next iteration */
        a2 = an;
    }
    return a2;   /* a(n) */
}
