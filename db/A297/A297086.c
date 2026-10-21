/*
 * A297086  a(n) = 1 if gcd(n, phi(n)) == 1 otherwise 0.
 * https://oeis.org/A297086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [gcd(n,phi(n)) == 1]
 * Range:     u8 n <= 2633, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0, but we return 0 for safety. */
    if (n == 0)
        return 0;

    /* Convert index to seq_t for operations on arbitrary‑precision types. */
    seq_t n_seq = (seq_t)n;
    seq_t phi_n  = phi(n);          /* φ(n)  */
    seq_t g      = gcd(n_seq, phi_n); /* gcd(n, φ(n)) */

    return (g == 1) ? 1 : 0;        /* 1 → true, 0 → false */
}
