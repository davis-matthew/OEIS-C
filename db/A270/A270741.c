/*
 * A270741  a(n) = (Fibonacci(n+2)-1) mod Fibonacci(floor(n/2)).
 * https://oeis.org/A270741
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Fibonacci(n+2)-1 ) mod Fibonacci( floor(n/2) )
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 50, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

static void fib_pair_mod_rec(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0 % mod;
        *fn1 = 1 % mod;
        return;
    }

    seq_t a, b;
    fib_pair_mod_rec(n >> 1, mod, &a, &b);   /* a = F_k  mod, b = F_{k+1} mod */

    /* F_{2k} = F_k * (2*F_{k+1} - F_k) */
    seq_t two_b = b + b;
    seq_t diff;
    if (two_b < a) {
        diff = two_b + mod - a;
    } else {
        diff = two_b - a;
    }
    seq_t c = (a * diff) % mod;          /* F_{2k}  mod */

    /* F_{2k+1} = F_k^2 + F_{k+1}^2 */
    seq_t d = (a * a + b * b) % mod;     /* F_{2k+1} mod */

    if ((n & 1) == 0) {                  /* even n = 2k */
        *fn  = c;
        *fn1 = d;
    } else {                             /* odd n = 2k+1 */
        *fn  = d;
        *fn1 = (c + d) % mod;            /* F_{2k+2} = F_{2k} + F_{2k+1} */
    }
}

seq_t a(idx_t n)
{
    /* n >= 2 according to the problem statement */
    idx_t k = n >> 1;                     /* floor(n/2) */

    seq_t mod = fib(k);                    /* Fibonacci( floor(n/2) )  */
    /* mod is at least 1 for n >= 2 */

    seq_t fn;  /* will hold Fibonacci(n+2) modulo mod */
    seq_t dummy;   /* unused second value of the pair */
    fib_pair_mod_rec(n + 2, mod, &fn, &dummy);

    /* (F_{n+2} - 1) mod mod */
    seq_t result;
    if (fn < 1) {
        result = (fn + mod - 1) % mod;
    } else {
        result = (fn - 1) % mod;
    }
    return result;
}
