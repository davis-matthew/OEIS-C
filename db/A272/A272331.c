/*
 * A272331  Refactorable Fibonacci numbers.
 * https://oeis.org/A272331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   seq_from_str
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
#include "../utils/seq_from_str.h"

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
