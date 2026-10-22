/*
 * A300409  Number of centered triangular numbers dividing n.
 * https://oeis.org/A300409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d | d is a centered triangular number and d | n}
 * Range:     u8 n <= 4325, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A centered triangular number is T_k = 3*k*(k-1)/2 + 1 for k ≥ 1. */

#include <stdbool.h>  /* for bool, unused but harmless */

/* External types seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* n is ≥ 1 for the sequence, but handle n == 0 gracefully. */
    if (n == 0)
        return (seq_t)0;

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;  /* use seq_t for arithmetic and modulus */

    for (idx_t k = 1; ; ++k)
    {
        /* Compute centered triangular number: T_k = 3*k*(k-1)/2 + 1 */
        seq_t term = ((seq_t)3 * (seq_t)k * (seq_t)(k - 1)) / 2 + 1;

        if (term > n_seq)
            break;

        if (n_seq % term == 0)
            ++count;
    }

    return count;
}
