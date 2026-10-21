/*
 * A265431  Total number of OFF (white) cells after n iterations of the "Rule
 *          188" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/16*(10*n^2 + 8*n + 3*(-1)^n - 2*(-i)^n - 2*i^n + 1)
 * Range:     u8 n <= 19, u16 n <= 323, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 N = n;
    unsigned __int128 E = 10 * N * N + 8 * N + 1;

    /* add 3*(-1)^n */
    if ((n & 1ULL) == 0)
        E += 3;          /* n even */
    else
        E -= 3;          /* n odd (E >= 3 for all odd n) */

    /* add the (–2*(-i)^n – 2*i^n) part:
       it equals –4 when n mod 4 == 0,
       0 when n mod 4 == 1 or 3,
       +4 when n mod 4 == 2   */
    unsigned rem4 = n & 3ULL;
    if (rem4 == 0)
        E -= 4;          /* n mod 4 == 0 */
    else if (rem4 == 2)
        E += 4;          /* n mod 4 == 2 */

    /* E is divisible by 16; convert to seq_t and return */
    return (seq_t)(E >> 4);
}
