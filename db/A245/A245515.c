/*
 * A245515  a(n) = n*floor(mod((gcd(n, Fibonacci((-1)^n + n))), 1 + n)/n) for
 *          n>=2.
 * https://oeis.org/A245515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*floor(mod(gcd(n, Fibonacci((-1)^n + n)), 1 + n)/n) for
 *            n>=1 The sequence equals n if n divides the appropriate
 *            Fibonacci number, otherwise 0
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute Fibonacci(n) modulo mod using fast doubling (iterative) */
static uint64_t fib_mod_u64(idx_t n, uint64_t mod)
{
    if (mod == 1)            /* all results are 0 modulo 1 */
        return 0;
    if (n == 0)              /* Fib(0) = 0 */
        return 0 % mod;

    /* a = F(k), b = F(k+1) initially for k=0 */
    uint64_t a = 0, b = 1 % mod;
    /* iterate over bits of n from high to low */
    int bit = 63 - __builtin_clzll(n);
    for (int i = bit; i >= 0; --i) {
        /* Compute the next pair (F(2k), F(2k+1)) from (F(k), F(k+1)) */
        unsigned __int128 two_b = (unsigned __int128)b * 2;
        unsigned __int128 two_b_mod = two_b % mod;

        unsigned __int128 temp = two_b_mod + mod - a;
        temp %= mod;                     /* (2*b - a) mod mod */

        unsigned __int128 d128 = (unsigned __int128)a * temp;
        uint64_t d = (uint64_t)(d128 % mod);                 /* F(2k) */

        unsigned __int128 a2 = (unsigned __int128)a * a;
        unsigned __int128 b2 = (unsigned __int128)b * b;
        unsigned __int128 e128 = a2 + b2;
        uint64_t e = (uint64_t)(e128 % mod);                 /* F(2k+1) */

        if ((n >> i) & 1) {   /* if bit is 1, advance to F(2k+1) and F(2k+2) */
            a = e;
            b = (d + e) % mod;
        } else {              /* else advance to F(2k) and F(2k+1) */
            a = d;
            b = e;
        }
    }
    return a;                 /* F(n) mod mod */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used for the official offset */

    /* Determine the Fibonacci index: n+1 if n even, n-1 if n odd */
    idx_t fib_index = (n & 1) ? n - 1 : n + 1;

    uint64_t mod = (uint64_t)n;
    uint64_t fib_mod = fib_mod_u64(fib_index, mod);

    /* a(n) is n if n divides the Fibonacci number, otherwise 0 */
    return (fib_mod == 0) ? (seq_t)n : (seq_t)0;
}
