/*
 * A382608  Long leg of the unique primitive Pythagorean triple whose inradius
 *          is A000045(n) and such that its long leg and its hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A382608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*F(n)*(F(n)+1) where F(n) is Fibonacci(n) with F(0)=0
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                      /* a = F(k), b = F(k+1), k = n/2 */
    seq_t c = a * (((seq_t)2 * b) - a);            /* F(2k)   */
    seq_t d = a * a + b * b;                      /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fib_n, fib_next;
    fib_pair(n, &fib_n, &fib_next);
    return (seq_t)2 * fib_n * (fib_n + 1);
}
