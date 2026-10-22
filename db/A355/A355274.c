/*
 * A355274  Numbers having more even than odd digits when written in base 3.
 * https://oeis.org/A355274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   more_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: iterate over non‑negative integers and
 * select those with more even base‑3 digits (0 or 2) than odd digits (1).
 * a(1) = 0 is the first such number.
 * The function works for any integers n ≥ 1.
 */
#include <stdbool.h>
#include "../utils/more_even_digits.h"

seq_t a(idx_t n)
{
    /* Undefined for n = 0; return 0 as a safe fallback. */
    if (n == 0) return (seq_t)0;

    seq_t candidate = (seq_t)0;
    idx_t count = 0;

    while (1) {
        if (more_even_digits(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}
