/*
 * A337857  a(n) is the smallest positive integer m with no repeated digits
 *          such that A137564(n + m) = n, or a(n) = 0 if no m exists.
 * https://oeis.org/A337857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive integer m with no repeated digits such
 *            that compact(n + m) = n; if no such m exists, a(n) = 0
 * Range:     u8 n <= 25, u16 n <= 122, u32 n <= 449, u64 n <= 449,
 *            u128 n <= 449, big any n
 * Helpers:   compact_seq_t, distinct_digits_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author's implementation of A337857. */
#include <stdbool.h>
#include "../utils/compact_seq_t.h"
#include "../utils/distinct_digits_seq_t.h"

/* Recursive generator for numbers with distinct digits (in increasing order).
   len   – target length of the generated number
   pos   – current position being filled
   cur   – current numeric value built so far
   mask  – bitmask of digits already used in cur
   n     – target value for compact(n + m) == n
   out   – pointer to store the answer when found
   found – global flag to stop recursion when the answer is found. */
static bool _found;
static seq_t _answer;

static void gen_rec(int pos, int len, seq_t cur, uint16_t mask,
                    idx_t n)
{
    if (_found) return;
    if (pos == len) {
        seq_t m = cur;
        seq_t s = (seq_t)n + m;                /* n + m  */
        if (compact_seq_t(s) == (seq_t)n) {
            _answer = m;
            _found = true;
        }
        return;
    }
    for (int d = (pos == 0 ? 1 : 0); d <= 9; ++d) {
        if (mask & (1u << d)) continue;
        gen_rec(pos + 1, len, cur * 10 + d, mask | (1u << d), n);
        if (_found) return;
    }
}

/* Main sequence function.  Calls the recursive generator and returns the
   smallest suitable m or 0 if no m exists. */
seq_t a(idx_t n)
{
    if (!distinct_digits_seq_t((seq_t)n))
        return 0;                 /* n has repeated digits → impossible  */

    _found = false;
    _answer = 0;
    for (int len = 1; len <= 10; ++len) {     /* max 10 distinct digits */
        gen_rec(0, len, 0, 0u, n);
        if (_found) return _answer;
    }
    return 0;                 /* no suitable m found                      */
}
