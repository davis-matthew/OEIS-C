/*
 * A329008  a(n) = p(0,n), where p(x,n) is the strong divisibility sequence of
 *          polynomials based on sqrt(3) as in A327321.
 * https://oeis.org/A329008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = { (3^n + 1) / 4 if n is odd * { floor((3^n + 1) / (8 *
 *            lowbit(n/2))) if n is even * * where lowbit(k) = k & -k (the
 *            largest power of two dividing k). * This follows from the closed
 *            form for the odd indices * a(2k+1) = (3^(2k+1)+1)/4 and the
 *            observed pattern for the even * indices derived...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation of 3^exp using only the operators allowed
 * on seq_t (addition, subtraction, multiplication, division).
 * exp is an unsigned integer type idx_t. */
static seq_t pow3(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n & 1) {                /* odd n = 2k+1 */
        idx_t k = n >> 1;        /* (n-1)/2 */
        idx_t exp = (k << 1) + 1;/* 2k+1 = n itself */
        seq_t pow = pow3(exp);   /* 3^n */
        seq_t sum = pow + 1;
        return sum / 4;          /* exact division */
    } else {                    /* even n = 2k */
        idx_t k = n >> 1;        /* n/2 */
        seq_t pow = pow3(n);     /* 3^n */
        seq_t num = pow + 1;     /* numerator */

        /* compute lowbit(k) = k & -k */
        idx_t lowbit = k & (-(idx_t)k); /* bit trick for unsigned types */

        seq_t denom = 8 * (seq_t)lowbit; /* 8 * lowbit(k) */
        return num / denom;              /* integer (floor) division */
    }
}
