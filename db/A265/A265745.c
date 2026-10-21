/*
 * A265745  a(n) is the number of Jacobsthal numbers (A001045) needed to sum
 *          to n using the greedy algorithm.
 * https://oeis.org/A265745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of Jacobsthal numbers needed by the greedy
 *            algorithm to represent n. A_1 = 0, A_2 = 1, A_k =
 *            A_{k-1}+2*A_{k-2} for k≥3
 * Range:     u8 n <= 300, u16 n <= 9624, u32 n <= 9624, u64 n <= 9624,
 *            u128 n <= 9624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes for standard types. */
#include <stddef.h>
#include <stdint.h>

/* a(n) uses the greedy algorithm on the Jacobsthal sequence. */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0)
        return (seq_t)0;

    /* Jacobsthal numbers up to the largest one ≤ n.
       We need at most about 70 such numbers for n up to 2^64-1. */
    seq_t jacob[70];
    size_t len = 0;

    /* J0 = 0, J1 = 1 (the sequence used for greedy sum skips the 0). */
    jacob[len++] = (seq_t)0;
    jacob[len++] = (seq_t)1;

    /* Generate further terms until we exceed n. */
    seq_t prev = (seq_t)0;
    seq_t curr = (seq_t)1;
    while (curr <= (seq_t)n)
    {
        seq_t next = curr + (seq_t)2 * prev;
        prev = curr;
        curr = next;
        jacob[len++] = curr;
    }

    /* Greedy decomposition of n. */
    seq_t remaining = (seq_t)n;
    seq_t terms = (seq_t)0;

    while (remaining > (seq_t)0)
    {
        /* Find the largest Jacobsthal number ≤ remaining. */
        size_t idx = len - 1;
        while (jacob[idx] > remaining)
            --idx;          /* idx will eventually be ≥ 1 because remaining > 0. */

        remaining -= jacob[idx];
        ++terms;
    }

    return terms;
}
