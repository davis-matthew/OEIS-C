/*
 * A296767  Numbers whose base-60 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2983, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Brute‑force scan of all positive integers, checking whether
   the base‑60 digits have fewer rises than falls.  For the nᵗʰ term, the
   function returns the nᵗʰ smallest positive integer that satisfies the
   condition. */
#include <stdbool.h>
#include <stddef.h>

/* Return true iff the base‑60 representation of x has
   #rises < #falls (rises: d[i] < d[i+1] in the usual most‑significant
   to least‑significant order; falls: d[i] > d[i+1]). */
static bool fewer_rises_than_falls(seq_t x)
{
    /* Store base‑60 digits, least significant first. */
    unsigned char digits[64];
    size_t len = 0;
    const seq_t sixty = (seq_t)60;
    seq_t tmp = x;

    do {
        digits[len++] = (unsigned char)(tmp % sixty);
        tmp /= sixty;
    } while (tmp > 0);

    unsigned rises = 0, falls = 0;
    /* Compare digits from most significant to least significant. */
    for (size_t j = len; j-- > 1; ) {
        if (digits[j] < digits[j-1]) {
            ++rises;
        } else if (digits[j] > digits[j-1]) {
            ++falls;
        }
    }
    return rises < falls;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)1;
    idx_t found = 0;

    while (true) {
        if (fewer_rises_than_falls(x)) {
            ++found;
            if (found == n) {
                return x;
            }
        }
        ++x;           /* next integer */
    }
}
