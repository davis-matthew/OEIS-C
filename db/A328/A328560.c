/*
 * A328560  Numbers whose product of digits is a power of 10.
 * https://oeis.org/A328560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 166, u32 n <= 27446, u64 n <= 27446,
 *            u128 n <= 27446, big any n
 * Validated: standard, pass, 63 terms through n = 8549 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose product of digits is a power of 10.
   This function enumerates numbers using only the digits 1,2,4,5,8 in
   ascending numeric order and returns the n‑th one whose digit product
   equals 10^k for some k ≥ 0. */
#include <stddef.h>
#include <stdbool.h>

/* Recursive helper: builds numbers of a fixed length `len`.  `pos` is the
   current digit position (0‑based).  `current` holds the number formed so
   far.  `c2` and `c5` count the total exponents of 2 and 5 contributed
   by the digits chosen.  When `pos == len` the number is complete and
   it is counted if the exponents match. */
static void
build_digits(size_t pos, size_t len,
             seq_t current, unsigned c2, unsigned c5,
             idx_t n,
             idx_t *count, seq_t *result, bool *found)
{
    if (*found)
        return;

    if (pos == len) {
        if (c2 == c5) {
            (*count)++;
            if (*count == n) {
                *result = current;
                *found  = true;
            }
        }
        return;
    }

    /* Allowable digits in ascending order to preserve numeric order. */
    static const unsigned digits[5] = {1, 2, 4, 5, 8};
    for (unsigned di = 0; di < 5; ++di) {
        unsigned d   = digits[di];
        unsigned nc2 = c2, nc5 = c5;

        switch (d) {
            case 1: break;
            case 2: nc2++; break;
            case 4: nc2 += 2; break;
            case 5: nc5++; break;
            case 8: nc2 += 3; break;
        }

        build_digits(pos + 1, len, current * (seq_t)10 + (seq_t)d,
                     nc2, nc5, n, count, result, found);

        if (*found)
            return;
    }
}

seq_t a(idx_t n)
{
    idx_t  count  = 0;   /* how many valid numbers seen so far   */
    seq_t  result = 0;   /* value to return when `n` is reached */
    bool   found  = false;

    for (size_t len = 1; !found; ++len) {
        build_digits(0, len, (seq_t)0, 0, 0, n, &count, &result, &found);
    }
    return result;
}
