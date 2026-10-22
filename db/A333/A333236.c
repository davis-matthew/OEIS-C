/*
 * A333236  Largest digit in the decimal expansion of 1/n.
 * https://oeis.org/A333236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = max_{i≥0} (floor(10^i / n) mod 10)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the largest decimal digit appearing in 1/n */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, guard */
    seq_t maxdigit = (seq_t)(1 / n);   /* integer part of 1/n (1 when n=1) */
    idx_t rem = 1 % n;                /* current remainder */
    if (rem == 0) return maxdigit;    /* terminating decimal, e.g. n=1 */

    /* array marking visited remainders to detect the recurring cycle */
    uint8_t *seen = (uint8_t *)calloc((size_t)n, sizeof(uint8_t));
    if (!seen) return maxdigit;        /* fallback if allocation fails */

    while (rem != 0 && !seen[rem]) {
        seen[rem] = 1;
        /* compute next digit: floor(rem*10 / n) */
        unsigned __int128 prod = (unsigned __int128)rem * 10U;
        seq_t digit = (seq_t)(prod / n);
        if (digit > maxdigit)
            maxdigit = digit;
        rem = (idx_t)(prod % n);
        if (maxdigit == 9) {           /* cannot get larger than 9 */
            free(seen);
            return maxdigit;
        }
    }
    free(seen);
    return maxdigit;
}
