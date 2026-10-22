/*
 * A343068  Multiplicative with a(p^e) = e*a(p-1).
 * https://oeis.org/A343068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343068: multiplicative with a(1)=1 and a(p^e)=e*a(p-1) */
/* The following implementation computes a(n) recursively by prime factorisation.
   It works for any seq_t (unsigned integer or arbitrary‑precision class) and
   for any idx_t (uint64_t). No dynamic allocation of seq_t objects is performed. */

#include <stddef.h>

/* Recursive multiplicative function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* base case: a(1)=1 */

    seq_t result = (seq_t)1;              /* multiplicative identity */
    idx_t m = n;

    /* trial division prime factorisation */
    for (idx_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t e = 0;
            while (m % d == 0) {          /* count exponent e of prime d */
                m /= d;
                ++e;
            }
            seq_t sub = a(d - 1);          /* a(p-1) for the prime d */
            seq_t mult = (seq_t)e * sub;   /* a(p^e) = e * a(p-1) */
            result *= mult;                /* combine multiplicative parts */
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1) {
        seq_t sub = a(m - 1);              /* a(p-1) for the remaining prime m */
        result *= sub;                     /* exponent e=1 contributes sub */
    }

    return result;
}
