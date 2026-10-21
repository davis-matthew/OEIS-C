/*
 * A296710  Numbers whose base-9 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A296710: a(n) = the n‑th integer whose base‑9 digits have more rises
    (d(i) < d(i+1)) than falls (d(i) > d(i+1) ), where the digits are
    taken from most significant to least significant.  */
#include <stdbool.h>

static bool good(seq_t v)
{
    if (v == 0) return false;          /* no digits, cannot satisfy rises>falls */

    /* store base‑9 digits least significant first */
    unsigned char tmp[128];
    int len = 0;
    seq_t x = v;
    while (x > 0) {
        tmp[len++] = (unsigned char)(x % 9);
        x /= 9;
    }

    /* reverse to most significant first: d[0] most significant */
    unsigned char d[128];
    for (int i = 0; i < len; ++i)
        d[i] = tmp[len - 1 - i];

    int rises = 0, falls = 0;
    for (int i = 0; i < len - 1; ++i) {
        if (d[i] < d[i + 1]) ++rises;
        else if (d[i] > d[i + 1]) ++falls;
    }
    return rises > falls;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 1;

    while (1) {
        if (good(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
}
