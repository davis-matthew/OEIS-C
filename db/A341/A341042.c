/*
 * A341042  Multiplicative projection of odd part of n.
 * https://oeis.org/A341042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over odd prime powers p^e of e*p. Equivalently:
 *            a(n)=a(n/2) if n is even; if n is odd, a(n)=∏_{p^e∥n} (e*p)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    idx_t odd = n;
    while ((odd & 1ULL) == 0) odd >>= 1;          /* remove factors of 2 */
    seq_t result = 1;
    idx_t p = 3;
    /* factor the odd part by trial division */
    for (; p <= odd / p; p += 2) {
        int e = 0;
        while ((odd % p) == 0) {
            odd /= p;
            e++;
        }
        if (e)
            result *= (seq_t)e * (seq_t)p;
    }
    if (odd > 1)             /* remaining prime factor (exponent 1) */
        result *= (seq_t)odd;
    return result;
}
