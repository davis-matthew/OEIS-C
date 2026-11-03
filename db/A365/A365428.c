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
 * Range:     i8 n <= 100000, i16 n <= 100000, i32 n <= 100000,
 *            i64 n <= 100000, i128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9957 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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
