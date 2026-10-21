/*
 * A243007  a(n) = A084769(n)^2.
 * https://oeis.org/A243007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{k=0..n} 4^k·C(2k,k)·C(n+k,n-k))^2 The inner sum is
 *            A084769(n); this recurrence for b(n)=A084769(n) follows from the
 *            differential equation of its generating function. b(0)=1,
 *            b(1)=9, b_{n+1} = ((18·n+9)·b_n − n·b_{n-1})/(n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* short‑circuit for the first two terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)81;   /* 9^2 */

    /* b(n) will hold the current value of A084769(n) */
    seq_t b_prev = (seq_t)1;   /* b(0) */
    seq_t b_curr = (seq_t)9;   /* b(1) */

    /* iterate the recurrence to reach b(n) */
    for (idx_t i = 1; i < n; ++i)  /* i represents n in the formula */
    {
        seq_t term1 = b_curr;
        term1 *= (seq_t)(18 * i + 9);        /* (18·i+9)·b_i */
        seq_t term2 = b_prev;
        term2 *= (seq_t)i;                  /* i·b_{i-1} */
        seq_t numerator = term1;
        numerator -= term2;                 /* ((18·i+9)·b_i − i·b_{i-1}) */
        /* divide by (i+1) – division is exact */
        seq_t next = numerator / (seq_t)(i + 1);

        b_prev = b_curr;
        b_curr = next;
    }

    /* a(n) = b(n)^2 */
    seq_t result = b_curr;
    result *= b_curr;
    return result;
}
