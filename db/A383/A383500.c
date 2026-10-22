/*
 * A383500  Positive integers that contain the digit 9, or an odd digit d
 *          immediately preceded by a digit <= d.
 * https://oeis.org/A383500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence consists of the positive integers that
    – contain the digit 9, or
    – contain an odd digit d immediately preceded by a digit
      ≤ d.
    The function a(n) returns the n-th such integer (n ≥ 1).  */

#include <stdbool.h>

static bool qualifies(seq_t x)
{
    seq_t t = x;
    unsigned char digits[256];     /* enough for any number <10^256 */
    int len = 0;

    /* Extract decimal digits (least‑significant first) */
    while (t > 0) {
        unsigned char d = (unsigned char)(t % 10);
        if (d == 9)          /* contains a 9 → qualifies */
            return true;
        digits[len++] = d;
        t /= 10;
    }

    /* Check adjacency rule:  right digit is odd and
       left digit (preceding) <= right digit. */
    for (int i = len - 1; i > 0; --i) {
        unsigned char left  = digits[i];   /* digit to the left */
        unsigned char right = digits[i - 1];/* digit to the right */
        if ((right & 1) && left <= right)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t cand  = 1;      /* current candidate integer   */
    idx_t count = 0;      /* how many qualifying numbers found */

    while (count < n) {
        if (qualifies(cand))
            ++count;
        if (count == n)
            return cand;
        ++cand;
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
