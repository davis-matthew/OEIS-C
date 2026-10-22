/*
 * A351100  Maximum number of 4-subsets of an n-set such that every 3-subset
 *          is covered at most twice.
 * https://oeis.org/A351100
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   * Let B(n) = ⌊n·(n-1)·(n-2)/12⌋ = ⌊(n choose 3)/2⌋. * Define k =
 *            ⌊n/6⌋ and r = n mod 6. * Then f(n) = * r == 0 → k * r == 3 → 2 *
 *            r == 1 or r == 5 → 2 · (k mod 2) * otherwise (r == 2 or r == 4)
 *            → 0 * The sequence value is a(n) = B(n) – f(n). * This formula
 *            reproduces all listed terms and matches...
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* compute B(n) = floor(n*(n-1)*(n-2)/12) */
    seq_t nn   = (seq_t)n;
    seq_t val  = nn * (nn - 1) * (nn - 2);
    seq_t B    = val / (seq_t)12;

    idx_t k = n / 6;          /* floor(n/6) */
    idx_t r = n % 6;          /* remainder modulo 6 */

    seq_t f = 0;
    if (r == 0)
        f = (seq_t)k;
    else if (r == 3)
        f = 2;
    else if (r == 1 || r == 5)
        f = 2 * (k & 1);     /* k mod 2 */

    return B - f;
}
