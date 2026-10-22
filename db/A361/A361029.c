/*
 * A361029  a(n) = 120*(3*n)!/(n!*(n+2)!^2).
 * https://oeis.org/A361029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*(3*n)!/(n!*(n+2)!^2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+2)^2*a(n) = 3*(3n-1)*(3n-2)*a(n-1), a(0)=30 */

#include <stdint.h>

/* Euclidean gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Computes a(n) of sequence A361029 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)30;

    seq_t val = (seq_t)30; /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        /* Build numerator and denominator without overflow */
        idx_t num1 = 3 * i - 1;   /* 3*i - 1 */
        idx_t num2 = 3 * i - 2;   /* 3*i - 2 */
        idx_t num  = 3;
        num = num * num1;          /* 3*(3i-1)  */
        num = num * num2;          /* 3*(3i-1)*(3i-2) */

        idx_t den = i + 2;
        den = den * den;           /* (i+2)^2 */

        /* Cancel common factors between num and den */
        idx_t g = gcd_idx(num, den);
        num /= g;
        den /= g;

        /* After cancellation, den divides current value exactly */
        val = val / den;           /* exact division */
        val = val * num;           /* multiply numerator */
    }
    return val;
}
