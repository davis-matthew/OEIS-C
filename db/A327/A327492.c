/*
 * A327492  Partial sums of A327491.
 * https://oeis.org/A327492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - popcount(n) + (n mod 2)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A327492.  */
seq_t a(idx_t n) {
    /* popcount of n (number of 1-bits) */
    unsigned int pop = __builtin_popcountll((uint64_t)n);
    /* 2*n, n mod 2 are computed in idx_t, then cast to seq_t for the final expression */
    seq_t result = (seq_t)(2 * n) - (seq_t)pop + (seq_t)(n & 1);
    return result;
}
