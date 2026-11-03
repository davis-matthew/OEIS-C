/*
 * A249830  Natural numbers having no valid Luhn mod 10 check digit.
 * https://oeis.org/A249830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9248 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th natural number that fails the Luhn checksum
   (i.e., its Luhn checksum modulo 10 is not 0). */

#include <stdbool.h>

static inline bool luhn_valid(seq_t x)
{
    int sum = 0;
    int pos = 1;                 /* rightmost digit = position 1 */
    while (x > 0) {
        int digit = (int)(x % 10);
        if (pos % 2 == 0) {      /* double every second digit */
            int dbl = digit * 2;
            sum += (dbl > 9) ? (dbl - 9) : dbl;
        } else {
            sum += digit;
        }
        x /= 10;
        pos++;
        sum %= 10;               /* keep sum small */
    }
    return (sum % 10 == 0);       /* true if the number itself is Luhn‑valid */
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t candidate = 1;
    while (found < n) {
        if (!luhn_valid((seq_t)candidate)) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
    return (seq_t)0;              /* unreachable */
}
