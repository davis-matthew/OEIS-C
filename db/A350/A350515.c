/*
 * A350515  a(n) = (n-1)/3 if n mod 3 = 1; a(n) = n/2 if n mod 6 = 0 or n mod
 *          6 = 2; a(n) = (3n+1)/2 if n mod 6 = 3 or n mod 6 = 5.
 * https://oeis.org/A350515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)/3 if n mod 3 = 1 * a(n) = n/2 if n mod 6 = 0 or 2 *
 *            a(n) = (3n+1)/2 if n mod 6 = 3 or 5
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t r3 = n % (idx_t)3;
    if (r3 == (idx_t)1)
        return ((seq_t)(n - (idx_t)1)) / (seq_t)3;

    const idx_t r6 = n % (idx_t)6;
    if (r6 == (idx_t)0 || r6 == (idx_t)2)
        return ((seq_t)n) / (seq_t)2;

    /* r6 == 3 or 5 */
    return ((seq_t)((seq_t)3 * n + (idx_t)1)) / (seq_t)2;
}
