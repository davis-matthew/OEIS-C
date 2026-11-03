/*
 * A307118  a(1) = 0; for n>1, a(n) = dr(n-1) + dr(n) + dr(n+1), where dr(n)
 *          is the number of nontrivial divisors of n (A070824).
 * https://oeis.org/A307118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = dr(n-1) + dr(n) + dr(n+1)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* dr(n) = number of non‑trivial divisors of n (A070824)   */

#include <stdint.h>

/* Count non‑trivial divisors of n.  For n <= 1 the result is 0.  */
static inline seq_t
dr(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Factor n by trial division – O(√n).  The number of divisors is a
       product over prime powers: prod (e_i + 1).  After computing that,
       remove the two trivial divisors 1 and n.  */
    seq_t divcnt = 1;
    idx_t   m     = n;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m    /= p;
                exp += 1;
            }
            divcnt *= (seq_t)(exp + 1);
        }
    }
    if (m > 1)                 /* remaining prime factor */
        divcnt *= 2;           /* exponent 1 -> factor (1+1) = 2 */

    if (divcnt <= 2)           /* less than two non‑trivial divisors */
        return 0;
    return divcnt - 2;
}

/* Sequence a(n).  External linkage is required by the build. */
seq_t
a(idx_t n)
{
    if (n == 1)
        return 0;
    return dr(n - 1) + dr(n) + dr(n + 1);
}
