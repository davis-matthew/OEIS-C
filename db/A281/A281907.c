/*
 * A281907  a(n) = 66483034025018711639862527490*n +
 *          47867742232066880047611079.
 * https://oeis.org/A281907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66483034025018711639862527490*n +
 *            47867742232066880047611079
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 7, big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    static seq_t coeff;
    static seq_t constant;
    static bool initialized = false;
    if (!initialized) {
        coeff = seq_from_str("66483034025018711639862527490");
        constant = seq_from_str("47867742232066880047611079");
        initialized = true;
    }
    seq_t result = coeff * (seq_t)n;
    result += constant;
    return result;
}
