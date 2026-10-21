/*
 * A260440  Unlucky numbers removed at the stage four of Lucky sieve.
 * https://oeis.org/A260440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n-1)%4] + 126*((n-1)/4), where base = {27, 57, 91,
 *            121}. This generates all numbers congruent to 27, 57, 91, or 121
 *            modulo 126, which is the definition of A260440
 * Range:     u8 n <= 8, u16 n <= 2080, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1; return 0 as placeholder */
        return 0;

    /* Constants for the four residue classes modulo 126. */
    static const seq_t base[4] = {27, 57, 91, 121};

    idx_t r = (n - 1) % 4;      /* index 0..3 selecting the residue class */
    idx_t k = (n - 1) / 4;      /* number of full 126‑periods preceding n */

    return base[r] + ((seq_t)126 * (seq_t)k);
}
