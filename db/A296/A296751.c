/*
 * A296751  Numbers whose base-13 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th integer whose base‑13 digit sequence has more rises
   than falls.  An integer is accepted if, when its base‑13 digits are
   ordered from most significant to least significant, the number of
   adjacent pairs (d[i],d[i+1]) with d[i] < d[i+1] is strictly larger than
   the number of pairs with d[i] > d[i+1]. */

#include <stdbool.h>

static bool good(seq_t x) {
    /* Store base‑13 digits (least significant first). */
    uint8_t digs[64];
    unsigned int len = 0;

    seq_t t = x;
    while (t > 0) {
        digs[len++] = (uint8_t)(t % 13);
        t /= 13;
    }
    /* Numbers with a single digit cannot have more rises than falls. */
    if (len <= 1) return false;

    int rises = 0, falls = 0;
    /* Compare digits from most significant to least significant. */
    for (int i = len - 1; i > 0; --i) {
        uint8_t cur  = digs[i];
        uint8_t next = digs[i - 1];
        if (cur < next)  ++rises;
        else if (cur > next) ++falls;
    }
    return rises > falls;
}

seq_t a(idx_t n) {
    seq_t candidate = 1;
    idx_t found = 0;
    while (1) {
        if (good(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
