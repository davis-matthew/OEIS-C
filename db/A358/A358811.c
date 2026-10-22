/*
 * A358811  Number of spanning trees in C_6 X C_n.
 * https://oeis.org/A358811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (A_n(1)-2)^2 * (A_n(3)-2)^2 * (A_n(4)-2) / 6 where
 *            A_n(λ) satisfies: A_0 = 2, A_1 = λ + 2, A_k = (λ + 2) * A_{k-1}
 *            - A_{k-2} for k ≥ 2 (The result is always an integer.)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t
compute_A(idx_t n, seq_t lambda)
{
    seq_t S = lambda + (seq_t)2;
    seq_t a0 = (seq_t)2;          /* A_0 */
    if (n == 0)
        return a0;
    seq_t a1 = S;                  /* A_1 = λ + 2 */
    if (n == 1)
        return a1;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = (S * a1) - a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute A_n(1), A_n(3), A_n(4) */
    seq_t A1 = compute_A(n, (seq_t)1);
    seq_t A3 = compute_A(n, (seq_t)3);
    seq_t A4 = compute_A(n, (seq_t)4);

    seq_t v1 = A1 - (seq_t)2;  /* A_n(1) - 2 */
    seq_t v3 = A3 - (seq_t)2;  /* A_n(3) - 2 */
    seq_t v4 = A4 - (seq_t)2;  /* A_n(4) - 2 */

    /* result = n * v1^2 * v3^2 * v4 / 6 */
    seq_t ns = (seq_t)n;
    seq_t res = ns * (v1 * v1) * (v3 * v3) * v4;
    res = res / (seq_t)6;       /* division is exact */

    return res;
}
