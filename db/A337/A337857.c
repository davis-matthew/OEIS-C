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
 * Validated: standard, pass, 76 terms through n = 441 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author's implementation of A337857. */
#include <stdbool.h>

/* Return the compact of x: the number obtained by removing all duplicate digits
   (keeping only first occurrence of each digit), in the same order. */
static seq_t compact_seq_t(seq_t x)
{
    seq_t result = 0;
    uint16_t seen = 0;
    /* store digits in reverse order */
    seq_t temp = x;
    seq_t digits[64];  /* enough for any realistic seq_t */
    int len = 0;
    while (temp > 0) {
        digits[len++] = temp % 10;
        temp /= 10;
    }
    for (int i = len - 1; i >= 0; --i) {
        int d = (int)(digits[i]);
        if (!(seen & (1u << d))) {
            result = result * 10 + d;
            seen |= (1u << d);
        }
    }
    return result;
}

/* Check whether seq_t x has all distinct decimal digits. */
static bool distinct_digits_seq_t(seq_t x)
{
    uint16_t mask = 0;
    if (x == 0) {
        /* digit 0 is considered distinct */
        return true;
    }
    while (x > 0) {
        int d = (int)(x % 10);
        if (mask & (1u << d)) return false;
        mask |= (1u << d);
        x /= 10;
    }
    return true;
}

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
