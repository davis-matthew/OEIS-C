/*
 * A286924  Positions of 1 in A286922; complement of A286923.
 * https://oeis.org/A286924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - floor((n+1)/sqrt(2))
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

/* Return the n-th term of A286924 (positions of 1 in A286922). */
seq_t a(idx_t n)
{
    if (n < 1) /* undefined for n < 1, return 0 as a safe default */
        return (seq_t)0;

    seq_t m      = (seq_t)(n + 1);      /* n+1 */
    seq_t m_sq   = m * m;               /* (n+1)^2  */
    seq_t half   = m_sq / 2;            /* floor((n+1)^2 / 2) */
    seq_t f      = isqrt_seq(half);     /* floor((n+1)/sqrt(2)) */
    seq_t result = ((seq_t)n * 2) - f;  /* 2*n - f */
    return result;
}
