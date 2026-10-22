/*
 * A382609  Semiperimeter of the unique primitive Pythagorean triple whose
 *          inradius is A000045(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A382609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fib(n) + 1) * (2*Fib(n) + 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth Fibonacci number by fast doubling.  */
static seq_t fib_fast(idx_t n)
{
    seq_t a = 0;  /* F(0) */
    seq_t b = 1;  /* F(1) */

    /* Count bits of n */
    int bits = 0;
    idx_t temp = n;
    while (temp) {
        bits++;
        temp >>= 1;
    }

    for (int i = bits - 1; i >= 0; i--) {
        /* c = F(k) * (2*F(k+1) - F(k))   */
        seq_t two_b_minus_a = (b << 1) - a;
        seq_t c = a * two_b_minus_a;
        /* d = F(k)^2 + F(k+1)^2         */
        seq_t d = a * a + b * b;

        if ((n >> i) & 1) {
            a = d;
            b = c + d;
        } else {
            a = c;
            b = d;
        }
    }
    return a; /* F(n) */
}

seq_t a(idx_t n)
{
    seq_t f = fib_fast(n);            /* Fibonacci(n)  */
    seq_t t1 = f + 1;                 /* Fib(n)+1      */
    seq_t t2 = (f << 1) + 1;          /* 2*Fib(n)+1    */
    return t1 * t2;                   /* (Fib(n)+1)*(2*Fib(n)+1) */
}
