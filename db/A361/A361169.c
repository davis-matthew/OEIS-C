/*
 * A361169  Discriminants D of Prym-Teichmuller curves W_D(4) in genus 3.
 * https://oeis.org/A361169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A361169: Discriminants D of Prym-Teichmuller curves W_D(4) in genus 3.
 * A(n) is the n-th integer >= 17 such that
 *   D % 8 ∈ {0, 1, 4}
 * and D is not a perfect square.
 * This implements a simple linear search; for n ≤ 10,000 the values stay well below
 * 64‑bit limits, so the routine runs in far less than 0.1 s.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1 – this case is irrelevant */

    idx_t count = 0;
    for (seq_t d = 17; ; ++d) {
        seq_t mod8 = d % 8;
        if (mod8 == 0 || mod8 == 1 || mod8 == 4) {
            seq_t s = isqrt_seq(d);
            if (s * s != d) {          /* discard perfect squares */
                ++count;
                if (count == n)
                    return d;
            }
        }
    }
}
