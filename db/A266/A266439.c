/*
 * A266439  Number of OFF (white) cells in the n-th iteration of the "Rule 23"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 when n ≤ 1 or n is odd; otherwise a(n) = 2*n + 1
 * Range:     u8 n <= 127, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1U)                // a(0) = a(1) = 0
        return (seq_t)0;

    if (n & 1U)                 // odd n → 0
        return (seq_t)0;

    /* even n ≥ 2: 2*n + 1 */
    /* cast intermediate results to seq_t to keep type‑safe */
    seq_t result = (seq_t)( (seq_t)2 * (seq_t)n + (seq_t)1 );
    return result;
}
