/*
 * A387032  Numbers k with digits different from 0 and 1.
 * https://oeis.org/A387032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose decimal digits are all in
 *            {2..9}
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, offset starts at 1 */

    /* Find the number of digits d such that the cumulative count of
       numbers with ≤d digits reaches n.  Encompasses 8^1 + 8^2 + … + 8^d. */
    idx_t cumulative = 0;
    idx_t mult = 8;               /* 8^1 */
    int d = 1;

    while (1) {
        unsigned __int128 total = (unsigned __int128)cumulative + mult;
        if (total >= n) break;    /* cumulative + mult >= n -> current digit length */
        cumulative = (idx_t)total;
        /* avoid overflow in mult *= 8 */
        if (mult > ((idx_t)(~(idx_t)0)) / 8) break;
        mult *= 8;
        d++;
    }

    /* Offset within d-digit block (0‑based). */
    idx_t offset = n - cumulative - 1;   /* 0 ≤ offset < 8^d */

    /* Extract base‑8 digits of offset, mapping 0→2, …, 7→9. */
    uint8_t digits[d];
    for (int i = 0; i < d; ++i) {
        digits[i] = (uint8_t)(offset % 8) + 2; /* least significant first */
        offset /= 8;
    }

    /* Build the decimal number from most significant to least. */
    seq_t res = 0;
    for (int i = d - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
