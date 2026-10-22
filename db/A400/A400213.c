/*
 * A400213  a(n) is the number of distinct areas of triangles formed by three
 *          vertices of a regular n-gon.
 * https://oeis.org/A400213
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = floor((n^2+3)/12) - [4 | n]*floor((n-4)/8) - [24 | n] -
 *            2*[30 | n]
 * Range:     u8 n <= 15, u16 n <= 885, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;                         /* offset starts at 3 */

    /* Compute A069905(n) = floor((n^2 + 3) / 12) */
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t A   = (sq + 3) / 12;

    /* Subtract equal‑area pairs */
    if (n % 4 == 0)          /* floor((n-4)/8) pairs for n divisible by 4 */
        A -= (n - 4) / 8;

    if (n % 24 == 0)         /* special pair for n divisible by 24 */
        A -= 1;

    if (n % 30 == 0)         /* two special pairs for n divisible by 30 */
        A -= 2;

    return A;
}
