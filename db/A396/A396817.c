/*
 * A396817  Total number of separating two-component spanning forests over all
 *          unordered pairs of rim vertices in the wheel graph W_n.
 * https://oeis.org/A396817
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (n-1)*((L_{2(n-1)}-2)*(F_{2⌊(n-1)/2⌋+1}-1)-F_{2(n-1)}*(L_{2⌊(n-1)/2⌋+1}-2⌊(n-1)/2⌋-1))
 *            * -
 *            ((1+(-1)^{n-1})/2)*(n-1)/2*(F_{n-1}*(L_{2(n-1)}-2)-F_{2(n-1)}*(L_{n-1}-2))
 *            * where F_k is the k‑th Fibonacci number and L_k is the k‑th
 *            Lucas number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Computes the Lucas number L_n as F_{n-1} + F_{n+1}.  */
static inline seq_t lucas(idx_t n)
{
    /* For n==0, the formula would access fib(-1); however this routine
       is never called with n==0 in this implementation. */
    return fib(n - 1) + fib(n + 1);
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 4; return 0 otherwise. */
    if (n < 4)
        return (seq_t)0;

    idx_t m = n - 1;               /* m = n-1  */
    idx_t q = m >> 1;              /* q = ⌊m/2⌋ */
    seq_t m_seq = (seq_t)m;        /* needed for multiplication */

    /* Pre‑compute all Fibonacci and Lucas values required. */
    seq_t L2m   = lucas(2 * m);
    seq_t F2m   = fib(2 * m);
    seq_t L2q1  = lucas(2 * q + 1);
    seq_t F2q1  = fib(2 * q + 1);
    seq_t Fm    = fib(m);
    seq_t Lm    = lucas(m);

    /* Inner part of the first large product.  */
    seq_t part1 = (L2m - (seq_t)2) * (F2q1 - (seq_t)1);
    seq_t part2 = F2m * (L2q1 - (seq_t)(2 * q) - (seq_t)1);
    seq_t term1 = m_seq * (part1 - part2);

    /* The second term appears only when m is even.  */
    seq_t term2 = (seq_t)0;
    if ((m & 1) == 0) {                     /* m even -> e_m = 1 */
        seq_t coeff = m_seq / (seq_t)2;      /* e_m * m / 2  */
        seq_t inner = Fm * (L2m - (seq_t)2)
                     - F2m * (Lm - (seq_t)2);
        term2 = coeff * inner;
    }

    return term1 - term2;
}
