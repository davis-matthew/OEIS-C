/*
 * A253179  a(n) = mu(n)*Sum_{k=1..n} (n/k) where mu(n) is Möbius (or Moebius)
 *          function and (x/y) is Kronecker's symbol.
 * https://oeis.org/A253179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(n)*∑_{k=1..n} (n/k)
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n/k) is Kronecker's symbol.  */

#include <stdint.h>
#include "../utils/mobius.h"

/* Jacobi symbol (a/n) for odd n ≥ 1.  Result ∈ {–1,0,1}. */
static int jacobi_idx(idx_t a, idx_t n)
{
    if (a == 0) return 0;
    if (a == 1) return 1;
    int result = 1;
    a %= n;
    while (a != 0) {
        /* remove factors of 2 from a */
        while ((a & 1) == 0) {
            a >>= 1;
            int n8 = n & 7;
            if (n8 == 3 || n8 == 5)
                result = -result;
        }
        /* swap a and n */
        idx_t tmp = a;
        a = n;
        n = tmp;
        /* quadratic reciprocity sign when both a and n ≡ 3 (mod 4) */
        if ((a & 3) == 3 && (n & 3) == 3)
            result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

/* Kronecker symbol (a/b) for integer a and positive integer b.  */
static int kronecker(idx_t a, idx_t b)
{
    /* b == 0 is never needed for this sequence */
    if (b == 1) return 1;

    /* sign for negative a – not used here because a = n ≥ 1 */
    int sign = 1;
    if ((int64_t)a < 0) {
        sign = -1;
        a = (idx_t)(-(int64_t)a);
    }

    /* remove powers of 2 from denominator b */
    int exp = 0;
    idx_t bb = b;
    while ((bb & 1) == 0) {
        bb >>= 1;
        ++exp;
    }

    if (exp > 0) {
        /* if a even the symbol is 0 */
        if ((a & 1) == 0) return 0;

        /* (a/2) */
        int a8 = a & 7;
        int two = (a8 == 1 || a8 == 7) ? 1 : -1;

        /* raise to the power exp */
        if ((exp & 1) == 0)
            two = 1;

        /* odd part */
        a %= bb;
        int odd = (bb == 1) ? 1 : jacobi_idx(a, bb);

        return sign * two * odd;
    } else {
        /* b is odd */
        a %= b;
        int odd = jacobi_idx(a, b);
        return sign * odd;
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int s = kronecker((idx_t)n, k);
        sum += (seq_t)s;
    }
    seq_t mu = mobius(n);
    return mu * sum;
}
