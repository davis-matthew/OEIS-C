/*
 * A338920  a(n) is the number of times it takes to iteratively subtract m
 *          from n where m is the largest nonzero proper suffix of n less than
 *          or equal to the remainder until no further subtraction is
 *          possible.
 * https://oeis.org/A338920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A338920: a(n) = number of subtractions needed to reduce n
   by repeatedly subtracting the largest non‑zero proper suffix
   of n that does not exceed the current remainder.  */
#include <stdint.h>

seq_t a(idx_t n) {
    /* single‑digit numbers have no proper suffixes */
    if (n < 10)
        return 0;

    seq_t tn = (seq_t)n;
    seq_t suffixes[20];      /* at most 19 proper suffixes for 64‑bit n */
    int sz = 0;

    seq_t pow10 = 10;        /* 10^1 */
    seq_t temp = tn;
    while (temp >= 10) {
        seq_t s = tn % pow10;
        if (s != 0)
            suffixes[sz++] = s;
        pow10 *= 10;
        temp /= 10;
    }

    seq_t r = tn;
    seq_t count = 0;

    while (r > 0) {
        seq_t m = 0;
        for (int i = 0; i < sz; ++i) {
            if (suffixes[i] <= r && suffixes[i] > m)
                m = suffixes[i];
        }
        if (m == 0)
            break;
        r -= m;
        ++count;
    }
    return count;
}
