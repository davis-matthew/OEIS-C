/*
 * A371034  For n >= 1, a(n) = A004086(n) if A055483(n) = 1, otherwise a(n) =
 *          n / A055483(n).
 * https://oeis.org/A371034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = R(n) if gcd(n,R(n))==1 else n/gcd(n,R(n))
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A371034 */
#include "../utils/gcd.h"
#include "../utils/rev.h"

seq_t a(idx_t n) {
    idx_t rev_n = rev(n);                 /* decimal reversal of n */
    seq_t g     = gcd((seq_t)n, (seq_t)rev_n); /* gcd(n, R(n)) */
    if (g == (seq_t)1) return (seq_t)rev_n;    /* coprime case */
    return (seq_t)n / g;                       /* otherwise n/gcd */
}
