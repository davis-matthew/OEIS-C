/*
 * A329964  a(n) = (n!/floor(1+n/2)!)^2.
 * https://oeis.org/A329964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! / floor(1 + n/2)!)^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A329964.
   The function uses only integer arithmetic on the seq_t type. */
seq_t a(idx_t n)
{
    /* floor(1 + n/2) is equivalent to n/2 + 1 using integer division */
    idx_t k = n / 2 + 1;                 /* lower bound of the factorial quotient */
    seq_t prod = 1;                      /* product of numbers from k+1 to n */

    /* Multiply the sequence from (k+1) up to n, inclusive.
       If k+1 > n, the loop does not execute and prod stays 1. */
    for (idx_t i = k + 1; i <= n; ++i)
        prod *= (seq_t)i;

    return prod * prod;   /* square the result */
}
