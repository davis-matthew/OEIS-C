/*
 * A380823  Semiperimeter of the unique primitive Pythagorean triple whose
 *          inradius is A000032(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A380823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Lucas(n) + 1) * (2*Lucas(n) + 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute Fibonacci(n) and Fibonacci(n+1) via fast doubling.
 * F(0) = 0, F(1) = 1.  The sequence is stored in *fn  and *fn1.
 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;                /* a = F(k), b = F(k+1) for k = n/2   */
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k)   = F(k) * (2*F(k+1) - F(k))          */
    seq_t two_b = b + b;               /* 2*F(k+1) */
    seq_t c = a * (two_b - a);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2                 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);                    /* fn  = F(n),  fn1 = F(n+1) */

    /* Lucas(n) = 2*F(n+1) - F(n) */
    seq_t two_fn1 = fn1 + fn1;
    seq_t lucas   = two_fn1 - fn;

    /* a(n) = (lucas + 1) * (2*lucas + 1) */
    seq_t lucas_plus_two = lucas + lucas;
    return (lucas + 1) * (lucas_plus_two + 1);
}
