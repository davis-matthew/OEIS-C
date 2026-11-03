/*
 * A285052  Number of idempotent equivalence classes for multiplication in Zn.
 * https://oeis.org/A285052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^A001221(n) where A001221(n) is the number of distinct
 *            prime factors of n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   ipow_i32, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* For n >= 1. count_distinct_primes(1) returns 0, so a(1) = 4^0 = 1. */
    int w = omega((uint64_t)n);   /* ω(n) */
    return ipow_i32((seq_t)4, w);                  /* 4^ω(n) */
}
