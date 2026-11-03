/*
 * A376045  Complement of A325112.
 * https://oeis.org/A376045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9150 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376045: the n-th positive integer whose decimal expansion contains a subsequence that is divisible by 3. */
#include <stdint.h>
#include <stdbool.h>

/* Check whether a number has a decimal subsequence divisible by 3. */
static bool has_divisible_subseq(idx_t x)
{
    /* Count digits that are ≡1 or 2 (mod 3). */
    unsigned c1 = 0, c2 = 0;
    uint64_t y = x;          /* keep a copy for digit extraction */

    while (y) {
        unsigned d = y % 10;
        if (d == 3 || d == 6 || d == 9)          /* a single 3,6,9 already works */
            return true;
        if (d % 3 == 1)
            ++c1;
        else if (d % 3 == 2)
            ++c2;
        y /= 10;
    }

    /* A subsequence of digits sum to 0 (mod 3) ⇔ we have at least one digit 1 and
       one digit 2, or at least three digits ≡1, or at least three digits ≡2. */
    if ((c1 > 0 && c2 > 0) || c1 >= 3 || c2 >= 3)
        return true;
    return false;
}

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined by OEIS, but guard anyway */

    idx_t count = 0;
    idx_t num   = 1;

    while (true) {
        if (has_divisible_subseq(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;    /* cast to the output type */
        }
        ++num;
    }
}
