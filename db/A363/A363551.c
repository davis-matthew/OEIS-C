/*
 * A363551  Möbius function of rank 3: a(n) = lambda(n) = A008836(n) if n is
 *          cubefree and 0 otherwise.
 * https://oeis.org/A363551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (n is cubefree) ? λ(n) : 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where λ(n) is the Möbius function (A008836) */
/* Multiplicative: λ(p^e) = (-1)^e  for e≤2, 0 otherwise */

#include <stdint.h>

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* n ≥ 1 for valid calls – behaviour for n < 1 is irrelevant. */
    uint64_t m = (uint64_t)n;
    if (m == 1)
        return (seq_t)1;               /* λ(1) = 1, 1 is cubefree */

    int parity = 0;                    /* parity of the number of prime factors
                                         with odd exponents (i.e., total count
                                         of prime factors with odd exponent). */

    /* trial division – enough for n up to at least 10^12 within 2 s. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            do {                      /* count exponent of prime p */
                m /= p;
                ++e;
            } while (m % p == 0);

            if (e >= 3)               /* not cubefree → λ(n)=0 */
                return (seq_t)0;

            if (e & 1)                /* flip parity if exponent is odd */
                parity ^= 1;
        }
    }

    /* If m>1 now, it is a remaining prime factor with exponent 1. */
    if (m > 1)
        parity ^= 1;                  /* exponent 1 → odd */

    return parity ? (seq_t)-1 : (seq_t)1;
}
