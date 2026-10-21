/*
 * A248042  Denominators of approximation to 2^(1/3) by Newton's method after
 *          n iterations.
 * https://oeis.org/A248042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248042: Denominators of approximation to 2^(1/3)
 *
 * Formula:
 *   a(n) = y(n) / gcd(x(n), y(n))
 *   x(n) = 2*(B(n-1)^3*A(n-1)^2 + A(n-1)^5)
 *   y(n) = 3*B(n-1)^2*A(n-1)^3
 *   where B(k) = A248041(k) is the numerator of the k‑th Newton
 *   approximation, and A(k) = a(k) is its denominator.
 *
 * Initial values: a(0) = 1, B(0) = 1.
 *
 * The function memoises the two sequences so that subsequent
 * calls are answered in O(1).
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static idx_t max_n = (idx_t)-1;
    static seq_t *num = NULL;   /* Numerators A248041 */
    static seq_t *den = NULL;   /* Denominators A248042 */

    /* Initialise the sequences with the first term.  */
    if (max_n == (idx_t)-1) {
        max_n = 0;
        num = (seq_t*)malloc(sizeof(seq_t));
        den = (seq_t*)malloc(sizeof(seq_t));
        if (!num || !den) return (seq_t)0;
        num[0] = (seq_t)1;
        den[0] = (seq_t)1;
    }

    if (n <= max_n) return den[n];

    /* Grow the arrays to hold terms through n.  */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp;

    tmp = (seq_t*)realloc(num, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    num = tmp;

    tmp = (seq_t*)realloc(den, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    den = tmp;

    for (idx_t i = max_n + 1; i <= n; ++i) {
        idx_t prev = i - 1;
        seq_t a_prev = den[prev];    /* Previous denominator (A(n-1)) */
        seq_t b_prev = num[prev];    /* Previous numerator  (B(n-1)) */

        /* Compute b^3 + a^3 */
        seq_t b2 = b_prev * b_prev;
        seq_t b3 = b2 * b_prev;
        seq_t a2 = a_prev * a_prev;
        seq_t a3 = a2 * a_prev;
        seq_t sum = b3 + a3;

        /* Raw numerator and denominator before reduction */
        seq_t raw_num = sum * ((seq_t)2);          /* 2*(b^3 + a^3)   */
        seq_t raw_den = a_prev * b2;               /* a * b^2         */
        raw_den = raw_den * ((seq_t)3);            /* 3*a*b^2         */

        /* Reduce the fraction */
        seq_t g = gcd(raw_num, raw_den);
        num[i] = raw_num / g;
        den[i] = raw_den / g;
    }

    max_n = n;
    return den[n];
}
