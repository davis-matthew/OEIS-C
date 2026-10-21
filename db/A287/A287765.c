/*
 * A287765  Period 4: repeat [1, 3, 5, 3].
 * https://oeis.org/A287765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287765
   Formula: a(n) = ((n+3) mod 4) + ((n+4) mod 4)
   Recurrence: a(n) = a(n-1) - a(n-2) + a(n-3),  a(1)=1, a(2)=3, a(3)=5
   Period 4 cycle: [1, 3, 5, 3] */

seq_t a(idx_t n)
{
    /* For n == 0, the sequence is not defined; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the two modulo 4 components efficiently. */
    idx_t r1 = (n + 3ULL) & 3ULL;  /* (n+3) mod 4 */
    idx_t r2 = (n + 4ULL) & 3ULL;  /* (n+4) mod 4 */

    /* Sum gives the required term, which fits in seq_t. */
    return (seq_t)(r1 + r2);
}
