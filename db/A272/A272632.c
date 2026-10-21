/*
 * A272632  Non-Fibonacci numbers that are both a sum and a difference of two
 *          Fibonacci numbers.
 * https://oeis.org/A272632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n-1) = fib(n+1)+fib(n+3) a(2*n) = 2*fib(n+3) (n >= 1) where
 *            fib(0)=0, fib(1)=1
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* fast doubling for Fibonacci numbers, works for any seq_t that supports
   +, -, *, and (bit) shifts. */
static inline seq_t fib_fast(idx_t k)
{
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */
    if (k == 0) return a;

    /* find highest set bit */
    int shift = 63 - __builtin_clzll(k);
    for (int i = shift; i >= 0; --i)
    {
        /* c = F(2i)  = F(i)*(2*F(i+1)-F(i)) */
        seq_t c = a * ((b << 1) - a);
        /* d = F(2i+1)= F(i)^2 + F(i+1)^2 */
        seq_t d = a * a + b * b;

        if ((k >> i) & 1)
        {
            a = d;          /* next a = F(2i+1)   */
            b = c + d;      /* next b = F(2i+2)   */
        }
        else
        {
            a = c;          /* next a = F(2i)     */
            b = d;          /* next b = F(2i+1)   */
        }
    }
    return a; /* F(k) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index 0 is out of range, return 0 */
    if (n & 1)                     /* odd index: a(2*m-1) */
    {
        idx_t m = (n + 1) >> 1;    /* m = (n+1)/2 */
        return fib_fast(m + 1) + fib_fast(m + 3);
    }
    else                           /* even index: a(2*m)   */
    {
        idx_t m = n >> 1;          /* m = n/2 */
        return fib_fast(m + 3) * 2;
    }
}
