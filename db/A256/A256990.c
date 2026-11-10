/*
 * A256990  One-based row index of n in array A256995.
 * https://oeis.org/A256990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, if n is not in the image of f(m) = 2m -
 *            popcount(m) (A005187), a(n) = n - h where h = max{m : f(m) <= n}
 *            (its index in A055938); else n = f(h) and a(n) = a(h)
 * Range:     u8 n <= 400, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 105 terms through n = 7738 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static unsigned A256990_pop(seq_t x)
{
    unsigned c = 0;
    while (x != (seq_t)0) {
        x &= x - (seq_t)1;
        ++c;
    }
    return c;
}

static seq_t A256990_f(seq_t x)
{
    return (seq_t)2 * x - (seq_t)A256990_pop(x);
}

seq_t a(idx_t n_idx)
{
    seq_t n = (seq_t)n_idx;
    while (true) {
        if (n <= (seq_t)1) return (seq_t)0;
        /* largest h with f(h) <= n; f is strictly increasing */
        seq_t lo = n / 2, hi = n;
        while (lo < hi) {
            seq_t mid = lo + (hi - lo + 1) / 2;
            if (A256990_f(mid) <= n) lo = mid;
            else hi = mid - 1;
        }
        if (A256990_f(lo) == n) {
            n = lo;
        } else {
            return n - lo;
        }
    }
}
