/*
 * A327008  a(n) = number of iterations of f(x)=floor((x^2+n^2)/(2x)) starting
 *          at x=n^2 to reach the value n.
 * https://oeis.org/A327008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of iterations of f(x)=floor((x^2+n^2)/(2x))
   starting at x=n^2 until the value n is reached. */
seq_t a(idx_t n)
{
    /* nsq = n^2 */
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t x   = nsq;          /* current value of x */
    seq_t steps = 0;

    while (x > (seq_t)n) {
        /* avoid overflow in 2*x */
        seq_t two_x = x << 1;          /* 2*x */

        seq_t halfx   = x / 2;          /* floor(x/2)          */
        seq_t div     = nsq / two_x;    /* floor(n^2/(2x))     */
        seq_t rem     = nsq % two_x;    /* n^2 mod (2x)        */

        seq_t nx = halfx + div;         /* candidate next x     */
        /* add 1 if x is odd and fractional part pushes up */
        if ((x & 1) && (rem >= x))
            ++nx;

        x = nx;
        ++steps;
    }

    return steps;
}
