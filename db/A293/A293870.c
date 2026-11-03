/*
 * A293870  Numbers having '10' as substring of their digits.
 * https://oeis.org/A293870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the nth integer containing the decimal substring "10".
   The implementation performs a linear scan from 10 upward, counting
   numbers whose base‑10 representation contains the pattern "10".
   For each candidate it examines the digits from most significant to
   least significant using modulo 10 and division.  This is O(L*M) where
   L is the number of candidates examined and M the number of digits in
   each candidate; for the typical n up to 10 000 the running time is
   well under 2 s on modern hardware. */

#include <stdbool.h>
#include <stdint.h>

/* Test whether decimal representation of x contains the substring "10" */
static inline bool has_substring_10(seq_t x)
{
    seq_t tmp = x;
    /* Extract digits into a stack (least significant first) */
    unsigned int digits[256];   /* enough for very long numbers */
    int len = 0;

    /* Handle the (unlikely) case x == 0 */
    if (tmp == (seq_t)0) {
        return false;
    }

    while (tmp != (seq_t)0) {
        digits[len++] = (unsigned int)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* Scan from most significant to least significant */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 1 && digits[i - 1] == 0) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t num = (seq_t)10;   /* the first number that can contain "10" */
    idx_t count = 0;

    while (1) {
        if (has_substring_10(num)) {
            ++count;
            if (count == n) {
                return num;
            }
        }
        ++num;                   /* advance to next integer */
    }
}
