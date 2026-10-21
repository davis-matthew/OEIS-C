/*
 * A291454  Number of half tones between successive pitches in a major scale.
 * https://oeis.org/A291454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   a(n) = floor(12*(n+1)/7) - floor(12*n/7)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 the behaviour
       is irrelevant; we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the floor divisions using unsigned 64‑bit arithmetic
       to avoid overflow of small seq_t types.  The result fits in
       a seq_t since the values are only 1 or 2. */
    idx_t v1 = (idx_t)12 * (n + 1) / 7;  /* floor(12*(n+1)/7) */
    idx_t v2 = (idx_t)12 * n / 7;        /* floor(12*n/7) */

    return (seq_t)(v1 - v2);
}
