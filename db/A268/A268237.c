/*
 * A268237  From the Fouriest transform of n: write n in that base b >= 4
 *          which maximizes the number of 4's; in case of a tie pick the
 *          smallest b; sequence gives the base b.
 * https://oeis.org/A268237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1051, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268237: Fouriest transform of n
 * a(n) = smallest base b >= 4 that maximizes the number of 4's
 * in the base-b representation of n (ties broken by smallest b).
 * If no base gives any 4's, a(n) = 4.
 *
 * The function iterates over all bases b = 4..n+1,
 * counts the occurrences of digit 4 in the representation of n,
 * and selects the base with the maximum count.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n < 4 the definition gives a(n) = 4. */
    if (n < 4) {
        return (seq_t)4;
    }

    idx_t bestCount = 0;     /* maximum number of 4's seen so far. */
    idx_t bestBase  = 0;     /* corresponding base. */
    bool     found  = false;  /* true if we have seen any 4 at all. */

    /* Special case: include base n+1 to capture n=4 case (e.g., 4 in base 5). */
    idx_t maxB = n + 1;
    for (idx_t b = 4; b <= maxB; ++b) {
        idx_t temp = n;
        idx_t count = 0;
        while (temp > 0) {
            idx_t digit = temp % b;
            if (digit == 4U) {
                ++count;
            }
            temp /= b;
        }
        if (count > bestCount) {
            bestCount = count;
            bestBase = b;
            found = true;
        }
    }

    if (!found || bestCount == 0) {
        /* No base contains a digit 4.  By definition we return 4. */
        return (seq_t)4;
    } else {
        return (seq_t)bestBase;
    }
}
