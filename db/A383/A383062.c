/*
 * A383062  Nonnegative integers such that every odd digit except the leftmost
 *          is immediately preceded by a smaller digit.
 * https://oeis.org/A383062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383062: numbers whose odd digits other than the leftmost are
   immediately preceded by a smaller digit.  Indexing starts at 1:
   a(1)=0, a(2)=1, a(3)=2, ...  */
#include <stdbool.h>
#include <stddef.h>

/* Check whether a number satisfies the required property.  */
static bool is_valid(seq_t x) {
    /* 0 is always valid. */
    if (x == (seq_t)0) return true;

    /* Extract decimal digits (least significant first).  */
    unsigned char digits[80];
    idx_t cnt = 0;
    seq_t tmp = x;
    while (tmp != (seq_t)0) {
        seq_t next = tmp / (seq_t)10;
        seq_t d = tmp % (seq_t)10;          /* 0 <= d <= 9 */
        digits[cnt++] = (unsigned char)d;
        tmp = next;
        if (cnt >= 80) {                    /* safety limit */
            break;
        }
    }

    /* Test digits from most significant to least.  */
    /* digits[0] is least significant, digits[cnt-1] is most significant. */
    for (idx_t i = 1; i < cnt; ++i) {
        unsigned char curr = digits[cnt - 1 - i];
        unsigned char prev = digits[cnt - i];
        if ((curr & 1) && prev >= curr)  /* odd digit not preceded by a smaller digit */
            return false;
    }
    return true;
}

/* Return the nth term of the sequence (1‑based index).  */
seq_t a(idx_t n) {
    idx_t seen = 0;
    seq_t val = (seq_t)0;

    while (1) {
        if (is_valid(val)) {
            ++seen;
            if (seen == n) return val;
        }
        ++val;
    }
}
