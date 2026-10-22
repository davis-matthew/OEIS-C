/*
 * A365428  Dirichlet inverse of A102283.
 * https://oeis.org/A365428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1; a(p) = -Legendre(-3,p) for prime p; a(p^e) = 0 for e ≥
 *            2. Since a is multiplicative this gives a(n) = 0 if n is not
 *            squarefree, otherwise a(n) = Π_{p∣n} a(p). A(2)=1, A(3)=0, for
 *            p>3 a(p)=±1
 * Range:     i8 n <= 9957, i16 n <= 9957, i32 n <= 9957, i64 n <= 9957,
 *            i128 n <= 9957, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    idx_t temp = n;
    seq_t result = 1;

    /* Handle factor 2 separately (a(2)=1). */
    if ((temp & 1ULL) == 0) {
        temp >>= 1;                 /* divide by 2 once */
        if ((temp & 1ULL) == 0)      /* exponent ≥ 2 → not squarefree */
            return 0;
        /* a(2) = 1, so result unchanged */
    }

    /* Trial division for odd primes. */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                if (exp >= 2)          /* exponent ≥ 2 → not squarefree */
                    return 0;
            }
            /* Multiply by a(p) = -Legendre(-3,p). */
            int ls = jacobi_symbol((uint64_t)(p == 3 ? 0 : p - 3), (uint64_t)p);
            result *= (seq_t)(-ls);
        }
    }

    /* Remaining prime factor, if any. */
    if (temp > 1) {
        /* temp is a prime > sqrt(original temp) */
        int ls = jacobi_symbol((uint64_t)(temp == 3 ? 0 : temp - 3), (uint64_t)temp);
        result *= (seq_t)(-ls);
    }

    return result;
}
