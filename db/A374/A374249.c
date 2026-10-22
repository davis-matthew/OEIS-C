/*
 * A374249  Numbers k such that the k-th composition in standard order has its
 *          equal parts contiguous.
 * https://oeis.org/A374249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Brute force search over integers k. The nth term a(n) is the nth
 *            non‑negative integer k for which the k‑th composition in
 *            standard order (A066099) has all equal parts contiguous
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool good_k(uint64_t k)
{
    /* collect the positions of the 1‑bits (1‑indexed) */
    uint8_t diff[65];              /* differences of successive positions */
    int m = 0;                     /* number of ones */
    uint8_t last_pos = 0;          /* position of previous 1‑bit */

    uint64_t t = k;
    int i = 0;
    while (t) {
        if (t & 1ULL) {
            uint8_t pos = i + 1;               /* 1‑indexed position */
            diff[m++] = pos - last_pos;
            last_pos = pos;
        }
        t >>= 1;
        ++i;
    }

    /* test that all equal parts appear contiguously
       by scanning the parts in reverse order of diff[] */
    bool seen[65] = { false };
    int current_v = -1;

    for (int j = m - 1; j >= 0; --j) {
        int v = diff[j];
        if (v == current_v)            /* still inside a run of the same part */
            continue;
        if (seen[v])                   /* this part has appeared before */
            return false;
        if (current_v != -1)           /* previous run finished */
            seen[current_v] = true;
        current_v = v;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t remaining = n;          /* number of good k's still needed */
    uint64_t k = 0;
    while (1) {
        if (good_k(k)) {
            if (remaining == 1)
                return (seq_t)k;
            --remaining;
        }
        ++k;
    }
}
