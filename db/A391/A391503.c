/*
 * A391503  a(n) = Kronecker symbol (10/n).
 * https://oeis.org/A391503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (10/n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9726 (2026-11-01)
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
    /* Offset 0: a(0) = 0 */
    if (n == 0)
        return 0;

    /* Special case n = 1: (10/1) = 1 by definition */
    if (n == 1)
        return 1;

    /* If n is even or divisible by 5 the Kronecker symbol is 0 */
    if ((n & 1) == 0)       /* even → 0 */
        return 0;
    if (n % 5 == 0)         /* n ≡ 0 (mod 5) → 0 */
        return 0;

    /* For odd n not divisible by 5 we can use the Jacobi symbol */
    return (seq_t)jacobi_symbol(10, n);
}
