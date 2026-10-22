/*
 * A359632  Sequence of gaps between deletions of multiples of 7 in step 4 of
 *          the sieve of Eratosthenes.
 * https://oeis.org/A359632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence with period 8: 12, 7, 4, 7, 4, 7, 12, 3 */
seq_t a(idx_t n)
{
    /* The sequence is defined by a repeating cycle of length 8. */
    static const seq_t cycle[8] = {12, 7, 4, 7, 4, 7, 12, 3};
    /* Convert n (1‑based) to a 0‑based index into the cycle. */
    return cycle[(n - 1) % 8];
}
