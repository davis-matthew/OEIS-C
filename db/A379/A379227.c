/*
 * A379227  a(n) = 1 if A113177(n) is a multiple of 5, otherwise 0, where
 *          A113177 is fully additive with a(p) = Fibonacci(p).
 * https://oeis.org/A379227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9388, u16 n <= 9388, u32 n <= 9388, u64 n <= 9388,
 *            u128 n <= 9388, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A379227: a(n) = 1 if A113177(n) is a multiple of 5, else 0.
   A113177 is fully additive with a(p) = Fibonacci(p).  This function
   factors n, adds (exponent * Fib(p)) modulo 5 using the Fibonacci
   periodicity modulo 5 (period 20), and returns 1 if the sum is 0 mod 5.
   The implementation uses simple trial division; it works for any
   n that fits in idx_t and is fast enough for n ≤ 10000. */

static const unsigned char fib_mod5[20] = {
    0, 1, 1, 2, 3, 0, 3, 3, 1, 4,
    0, 4, 4, 3, 2, 0, 2, 2, 4, 1
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* the sequence is defined for n ≥ 1 */

    uint64_t m = (uint64_t)n;     /* factorize using 64‑bit arithmetic */
    int sum_mod5 = 0;             /* holds the running sum modulo 5 */

    /* factor 2 separately */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            e++;
        }
        sum_mod5 += e * fib_mod5[2];    /* Fib(2) mod 5 = 1 */
        sum_mod5 %= 5;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            unsigned idx = (unsigned)(p % 20);   /* Fibonacci period 20 mod 5 */
            sum_mod5 += e * fib_mod5[idx];
            sum_mod5 %= 5;
        }
    }

    /* if remaining m > 1 it is a prime factor */
    if (m > 1) {
        unsigned idx = (unsigned)(m % 20);
        sum_mod5 += fib_mod5[idx];
        sum_mod5 %= 5;
    }

    /* a(n) = 1 if the sum is 0 mod 5, else 0 */
    return (seq_t)(sum_mod5 == 0);
}
