/*
 * A260190  Kronecker symbol(-6 / 2*n + 1).
 * https://oeis.org/A260190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Kronecker(-6, 2*n + 1) (for n ≥ 0) Since 2*n+1 is an odd
 *            positive integer, the Kronecker symbol reduces to the Jacobi
 *            symbol. The only special case is m = 1, for which the Jacobi
 *            symbol is defined to be 1. Thus a(n) = (−1/m) · (6/m)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9232 (2026-11-01)
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
    /* m = 2*n + 1 is always odd and positive */
    uint64_t m = (uint64_t)2 * n + 1ULL;

    /* Special case: m == 1  →  Kronecker(-6,1) = 1 */
    if (m == 1ULL)
        return 1;

    /* Legendre symbol (−1/m) = (−1)^((m-1)/2) */
    int sign_minus_one = (m % 4 == 1) ? 1 : -1;

    /* Jacobi symbol (6/m)  –  provided by the utility header */
    int sign_six = jacobi_symbol(6, m);

    return (seq_t)(sign_minus_one * sign_six);
}
