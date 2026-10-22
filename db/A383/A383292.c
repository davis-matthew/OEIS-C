/*
 * A383292  Dirichlet g.f.: zeta(s) * Product_{p prime} (1 + 1/p^(2*s) +
 *          1/p^(3*s)).
 * https://oeis.org/A383292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383292
   Formula: a(n) = Π_{p^e‖n} a(p^e)  with  a(p^e) = e   if e < 4
                                                 = 3   otherwise
   i.e. for each prime power dividing n take the exponent e; if e<4 use e,
   else use 3; multiply all such values.  a(1) = 1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard for the unused offset.  a(1)=1 is naturally obtained. */
    if (n == 0)
        return 0;                 /* not needed, but keeps the function total */

    seq_t result = 1;
    idx_t m = n;

    /* Handle factor 2 separately for speed */
    if ((m & 1) == 0) {
        unsigned int e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        seq_t term = (e < 4) ? (seq_t)e : (seq_t)3;
        result *= term;
    }

    /* Odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = (e < 4) ? (seq_t)e : (seq_t)3;
            result *= term;
        }
    }

    /* Remaining prime factor (if any) has exponent 1 → term = 1 (no effect) */
    return result;
}
