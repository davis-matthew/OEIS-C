/*
 * A257810  Smallest number of the cycle in which n ends under iteration of
 *          sum-of-the-square-of-two-digits.
 * https://oeis.org/A257810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9543 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A257810
    a(n) = smallest element of the cycle reached by iterating the
    function   s_2(x) = sum of the squares of successive two‑digit
    groups (the first group may be a single digit if the number of
    decimal digits is odd).  */
#include <stdint.h>

static inline seq_t s2(seq_t v)
{
    /* Special case: v = 0 */
    if (v == 0)
        return 0;

    /* Extract decimal digits (least significant first) */
    unsigned char digs[64];
    idx_t len = 0;
    seq_t tmp = v;
    while (tmp > 0) {
        digs[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Process digits from most significant to least */
    seq_t sum = 0;
    idx_t pos = len - 1;            /* index of most significant digit */
    idx_t remaining = len;

    /* If the number of digits is odd, handle the first single digit */
    if ((remaining & 1) != 0) {
        seq_t d = (seq_t)digs[pos];
        sum += d * d;
        pos--;
        remaining--;
    }

    /* Process the remaining digits in two‑digit groups */
    while (remaining > 0) {
        seq_t high = (seq_t)digs[pos];
        seq_t low  = (seq_t)digs[pos - 1];
        seq_t group = high * 10 + low;
        sum += group * group;
        pos -= 2;
        remaining -= 2;
    }

    return sum;
}

seq_t a(idx_t n)
{
    /* Floyd’s cycle–finding algorithm for the iteration of s_2. */
    seq_t tortoise = s2((seq_t)n);
    seq_t hare     = s2(s2((seq_t)n));

    while (tortoise != hare) {
        tortoise = s2(tortoise);
        hare     = s2(s2(hare));
    }

    /* Find the first element of the cycle (cycle start). */
    tortoise = (seq_t)n;
    while (tortoise != hare) {
        tortoise = s2(tortoise);
        hare     = s2(hare);
    }
    seq_t cycle_start = tortoise;

    /* Determine the smallest value in the cycle. */
    seq_t min = cycle_start;
    seq_t cur = s2(cycle_start);
    while (cur != cycle_start) {
        if (cur < min)
            min = cur;
        cur = s2(cur);
    }

    return min;
}
