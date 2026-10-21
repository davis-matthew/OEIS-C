/*
 * A265537  Largest base-7 palindrome m <= n, written in base 10.
 * https://oeis.org/A265537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   gen_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest integer m ≤ n such that the base‑7 representation of m is a palindrome.
 * This implementation generates all base‑7 palindromes up to the length of n,
 * converts them to decimal, and keeps the maximum that does not exceed n.
 */

#include <stdint.h>                /* for uint8_t */
#include "../utils/gen_pal.h"

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 7)                /* single‑digit base‑7 numbers are palindromes */
        return (seq_t)n;

    /* Determine number of base‑7 digits of n. */
    int len = 0;
    idx_t tmp = n;
    do {
        tmp /= 7;
        ++len;
    } while (tmp);

    seq_t best = 0;
    uint8_t firstHalf[64];     /* enough for len up to 64 */

    for (int L = 1; L <= len; ++L) {
        int half = (L + 1) / 2;
        gen_pal(L, 0, half, firstHalf, n, &best);
    }

    return best;
}
