/*
 * A287324  a(n) = A008412(n-1) + A008412(n-2) for n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A287324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(2*n - 3)*(n^2 - 3*n + 5)/3 for n > 2, a(0) = 0, a(1) =
 *            1, a(2) = 9
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 715, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;

    /* Convert the index to seq_t once to avoid repeated casts. */
    seq_t ns = (seq_t)n;

    /* Compute (8 * (2*n - 3)) * (n^2 - 3*n + 5) / 3
       The intermediate values easily fit into 64‑bit even
       when seq_t is uint64_t, so no overflow occurs. */
    seq_t part1 = (seq_t)8 * ((seq_t)2 * ns - (seq_t)3);
    seq_t part2 = ns * ns - (seq_t)3 * ns + (seq_t)5;

    return (part1 * part2) / (seq_t)3;
}
