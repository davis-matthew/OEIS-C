/*
 * A385314  a(n) is the least positive integer m such that Sum_{k = 1 .. m}
 *          k^n is divisible by n.
 * https://oeis.org/A385314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least positive integer m such that Σ_{k=1}^{m} k^n ≡ 0 (mod n) */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* not in the domain, but guard against division by zero */
        return 0;

    seq_t mod  = (seq_t)n;   /* modulus */
    seq_t sum  = 0;          /* running sum modulo mod */

    for (idx_t m = 1;; ++m) {
        seq_t term = mod_pow((seq_t)m, n, mod);   /* m^n mod n */
        sum += term;                             /* sum < 2*mod */
        if (sum >= mod) sum -= mod;              /* keep sum in [0,mod) */
        if (sum == 0)                            /* found least m */
            return (seq_t)m;
    }
}
