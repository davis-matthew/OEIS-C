/*
 * A272331  Refactorable Fibonacci numbers.
 * https://oeis.org/A272331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: full, pass, 5 terms through n = 5 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272331 – Refactorable Fibonacci numbers.
   Luca & Young proved that there are no further terms;
   the sequence is finite and consists only of
   1, 2, 8, 46368, 4807526976.  For n > 5 the function
   returns 0.  This trivial implementation is the only
   practical way to compute the sequence in all valid
   builds. */

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
    /* The sequence starts at index 1.  For n <= 0 return 0. */
    if (n == 0) return seq_from_str("0");

    switch ((int)n) {
        case 1:  return seq_from_str("1");
        case 2:  return seq_from_str("2");
        case 3:  return seq_from_str("8");
        case 4:  return seq_from_str("46368");
        case 5:  return seq_from_str("4807526976");
        default: return seq_from_str("0");
    }
}
