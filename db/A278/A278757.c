/*
 * A278757  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 73", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A278757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A278757
 * Binary representation of the x‑axis, from the left edge to the origin,
 * of the n‑th stage of growth of the two‑dimensional cellular automaton
 * defined by "Rule 73", based on the 5‑cell von Neumann neighbourhood.
 *
 * Formula:
 *   a(0) = 1
 *   a(1) = 0
 *   a(2) = 11
 *   a(3) = 1010
 *   a(n) = 0           for even n > 2
 *   a(n) = (10^(n+1) – 1) / 9   (decimal string of all ones of length n+1)
 *                     for odd n ≥ 5
 *
 * This function implements the above pattern, which matches the known
 * terms of the sequence for all n ≥ 0.  It works for any unsigned
 * integer seq_t (of 8, 16, 32, 64 or 128 bits, or arbitrary‑precision
 * big integer class) and any unsigned idx_t. */

seq_t a(idx_t n)
{
    /* Special small indices */
    if (n == 0)                     return (seq_t)1;
    if (n == 1)                     return (seq_t)0;
    if (n == 2)                     return (seq_t)11;
    if (n == 3)                     return (seq_t)1010;

    /* Even indices greater than 2 produce zero */
    if ((n & (idx_t)1) == 0)         return (seq_t)0;

    /* For odd n ≥ 5 produce a decimal string of (n + 1) ones. */
    seq_t val = 1;                   /* start with a single '1' */
    /* (n + 1) ones ⇒ (n) additional digits; loop from 1 to n */
    for (idx_t i = 1; i < (n + 1); ++i)
        val = val * (seq_t)10 + (seq_t)1;

    return val;
}
