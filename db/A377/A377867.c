/*
 * A377867  Number of subwords of the form DDDD in nondecreasing Dyck paths of
 *          length 2n.
 * https://oeis.org/A377867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (3*(n-2)*L(2*n-4) - 3*F(2*n+1))/5 + (n+9)*2^(n-4)  (for n>=3);
 * a(0) = a(1) = a(2) = a(3) = 0.
 *
 * F(k) = Fibonacci(k)  (F(0)=0, F(1)=1)
 * L(k) = Lucas(k)      (L(k)=F(k-1)+F(k+1))
 *
 * The implementation avoids unsigned wrap‑around by
 *   1. Computing (n-2)*L(2n-4) with mul_small
 *   2. Subtracting the Fibonacci term with sign handling
 *   3. Multiplying the absolute difference by 3
 *   4. Dividing by 5
 *   5. Adding or subtracting the result from (n+9)*2^(n-4)
 */

#include <stddef.h>
#include "../utils/fib.h"
#include "../utils/pow2.h"
#include "../utils/mul_small.h"

static seq_t lucas(idx_t n)
{
    /* L(0)=2, L(1)=1; for n>=1, L(n)=F(n-1)+F(n+1) */
    if (n == 0) return (seq_t)2;
    return fib(n - 1) + fib(n + 1);
}

seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    /* (n+9)*2^(n-4) */
    seq_t pow_term = (seq_t)(n + 9) * pow2(n - 4);

    /* Lucas and Fibonacci terms */
    seq_t L = lucas(2 * n - 4);
    seq_t F = fib(2 * n + 1);

    /* (n-2)*L */
    seq_t termMul = mul_small(L, (unsigned)(n - 2));

    /* Determine sign of (termMul - F) */
    seq_t diff_abs;
    int sign_positive; /* 1 if termMul >= F, 0 otherwise */

    if (termMul >= F) {
        diff_abs = termMul - F;
        sign_positive = 1;
    } else {
        diff_abs = F - termMul;
        sign_positive = 0;
    }

    /* 3 * diff_abs and divide by 5 (the result is integer) */
    seq_t temp = mul_small(diff_abs, 3);
    seq_t diff_div5 = temp / (seq_t)5;

    if (sign_positive)
        return pow_term + diff_div5;
    else
        return pow_term - diff_div5;
}
