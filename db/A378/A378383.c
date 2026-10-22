/*
 * A378383  Number of subwords of the form UDDD in nondecreasing Dyck paths of
 *          length 2*n.
 * https://oeis.org/A378383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-3)*L(2n-5)+L(2n-3)+F(2n+2) -5*(n+5)*2^(n-4))/5 for
 *            n>=3 (with a(0)=a(1)=a(2)=0, a(3)=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Lucas numbers via L(k)=F(k-1)+F(k+1) */

#include "../utils/fib.h"
#include "../utils/pow2.h"

static seq_t lucas(idx_t k) {
    // Lucas: L(k) = F(k-1) + F(k+1), works for k >= 0
    return fib(k - 1) + fib(k + 1);
}

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    if (n == 3) return 1;

    /* n >= 4 */
    seq_t fn2   = fib(2 * n + 2);      /* F(2n+2) */
    seq_t l1    = lucas(2 * n - 5);    /* L(2n-5) */
    seq_t l2    = lucas(2 * n - 3);    /* L(2n-3) */

    seq_t term1 = l1 * (seq_t)(n - 3); /* (n-3)*L(2n-5) */

    /* 5*(n+5)*2^(n-4) */
    seq_t pow2_val = pow2(n - 4);              /* 2^(n-4) */
    seq_t mult     = (seq_t)(5 * (n + 5));     /* 5*(n+5) */
    seq_t term2    = pow2_val * mult;          /* product */

    seq_t numerator = term1 + l2 + fn2 - term2;
    return numerator / 5;
}
