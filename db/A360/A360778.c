/*
 * A360778  Smallest number k such that n + k is a refactorable number.
 * https://oeis.org/A360778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥ 0 | n + k is refactorable}
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether a positive integer n is refactorable.
   Refactorable numbers are divisible by their divisor count τ(n). */
static bool is_refactorable(seq_t n)
{
    if (n == (seq_t)0)          /* 0 is not considered refactorable in this definition */
        return false;

    seq_t tmp = n;
    seq_t tau  = 1;             /* divisor count */

    for (seq_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1)                /* remaining prime factor */
        tau *= 2;

    return (n % tau) == 0;
}

/* Smallest k such that n + k is a refactorable number.
   For n = 0, a(0) = 1 by definition. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t x = (seq_t)n;
    seq_t k = 0;

    while (!is_refactorable(x)) {
        ++x;
        ++k;
    }
    return k;
}
