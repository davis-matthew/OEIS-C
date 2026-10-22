/*
 * A330969  Numbers k such that for any decimal digit d in k, the remainder
 *          when k is divided by d+1 is d.
 * https://oeis.org/A330969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 2309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330969: numbers k such that for each decimal digit d in k,
 * k mod (d+1) == d.  Computed by brute‑force search. */

#include <stdbool.h>

static bool good(seq_t k)
{
    seq_t t = k;
    while (t != (seq_t)0) {
        int d = (int)(t % (seq_t)10);
        if ((int)(k % (seq_t)(d + 1)) != d)
            return false;
        t = t / (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)0;
    idx_t count = 0;
    while (1) {
        if (good(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        cur = cur + (seq_t)1;
    }
}
