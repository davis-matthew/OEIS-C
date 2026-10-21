/*
 * A269716  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 22", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1; a(1) = 5; for n ≥ 2, a(n) = 2^(n‑1)·(3·2^n – 2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0u) return 1;
    if (n == 1u) return 5;

    /* Compute 2^n as a sequence of multiplications by 2.   */
    seq_t pow2_n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2_n *= (seq_t)2;

    /* The formula: a(n) = (pow2_n / 2) * (3*pow2_n - 2). */
    seq_t part = 3 * pow2_n - 2;
    return (pow2_n >> 1) * part;
}
