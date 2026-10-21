/*
 * A270741  a(n) = (Fibonacci(n+2)-1) mod Fibonacci(floor(n/2)).
 * https://oeis.org/A270741
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Fibonacci(n+2)-1 ) mod Fibonacci( floor(n/2) )
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 50, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   fib, fib_pair_mod_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"   /* provides fib(idx_t) */
#include "../utils/fib_pair_mod_rec.h"

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
