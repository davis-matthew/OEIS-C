/*
 * A319441  Cubes of non-palindromic numbers.
 * https://oeis.org/A319441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A029742(n)^3
 * Range:     u8 none, u16 n <= 28, u32 n <= 1511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9739 (2026-11-01)
 * Helpers:   is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th non‑palindromic number in base 10 and returns its cube. */
#include <stdbool.h>
#include "../utils/is_palindrome_base.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t current = 10;                      /* first non‑palindromic integer */
    while (1)
    {
        if (!is_palindrome_base(current, 10))
        {
            ++count;
            if (count == n)
            {
                seq_t cube = current * current * current;
                return cube;
            }
        }
        ++current;
    }
}
