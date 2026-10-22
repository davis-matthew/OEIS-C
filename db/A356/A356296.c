/*
 * A356296  a(n) = Fibonacci(n)^2 mod n.
 * https://oeis.org/A356296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)^2 mod n
 * Range:     u8 n <= 16, u16 n <= 1087, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t sn = (seq_t)n;          /* n as seq_t for modulo arithmetic */

    /* Compute Fibonacci(n) modulo n (F(1)=1, F(2)=1) */
    seq_t f0 = 0;
    seq_t f1 = 1 % sn;            /* F(1) mod n */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t newf = f0 + f1;
        if (newf >= sn)           /* keep result < n */
            newf -= sn;
        f0 = f1;
        f1 = newf;
    }

    /* Return (Fibonacci(n)^2) mod n */
    seq_t sq = f1 * f1;
    seq_t res = sq % sn;
    return res;
}
