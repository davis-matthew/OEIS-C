/*
 * A318473  Additive with a(p^e) = A000045(e+1).
 * https://oeis.org/A318473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e‖n} F(e+1) where F is Fibonacci (A000045)
 * Range:     u8 n <= 8191, u16 n <= 8890, u32 n <= 8890, u64 n <= 8890,
 *            u128 n <= 8890, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses trial division to factor n.  It works
 * for any n ≥ 1, including the large values that may appear in
 * subsequent test‑cases.  The function is fully additive, so the
 * value for a product of coprime factors is the sum of the
 * corresponding values.  No global state is required.  */

#include <stdint.h>

#include "../utils/fib.h"   /* seq_t fib(idx_t n); */

seq_t a(idx_t n) {
    if (n == 1u)
        return 0;          /* a(1) = 0 */

    seq_t      res = 0;    /* result accumulator */
    idx_t      m   = n;    /* working copy */
    idx_t      exp = 0;    /* exponent of current prime */

    /* Handle factor 2 separately for speed. */
    while ((m & 1u) == 0u) {   /* while m is even */
        exp++;
        m >>= 1u;              /* divide by 2 */
    }
    if (exp)                    /* if any 2's were found */
        res += fib(exp + 1);

    /* Trial division over odd numbers.  The condition p <= m/p
     * is equivalent to p ≤ sqrt(m) but avoids overflow in p*p. */
    for (idx_t p = 3u; p <= m / p; p += 2u) {
        exp = 0;
        while (m % p == 0u) {  /* count exponent of p */
            exp++;
            m /= p;
        }
        if (exp)
            res += fib(exp + 1);
    }

    /* If m > 1 now, it is a remaining prime factor with exponent 1. */
    if (m > 1u)
        res += fib(2);        /* fib(2) = 1 */

    return res;
}
