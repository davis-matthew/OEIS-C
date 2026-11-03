/*
 * A318569  a(n) is the smallest positive integer such that n*a(n) is a
 *          "binary antipalindrome" (i.e., an element of A035928).
 * https://oeis.org/A318569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) is the smallest positive integer k such that n*k is a
 *            binary antipalindrome (a number with an even number of binary
 *            digits, and each bit is the opposite of its symmetric
 *            counterpart). The computation simply tests successive multiples
 *            of n until an antipalindrome is found
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9882 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether x is a binary antipalindrome. */
static bool is_antipalindrome(seq_t x)
{
    if (x == 0)
        return false;

    /* Compute bit length of x. */
    idx_t len = 0;
    seq_t tmp = x;
    while (tmp)
    {
        tmp >>= 1;
        ++len;
    }

    /* Length must be even. */
    if (len & 1)
        return false;

    idx_t L = len >> 1;
    for (idx_t i = 0; i < L; ++i)
    {
        seq_t low  = (x >> i) & (seq_t)1;
        seq_t high = (x >> (2 * L - 1 - i)) & (seq_t)1;
        /* Bits must be opposite; XOR should be 1. */
        if ((low ^ high) != (seq_t)1)
            return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    /* Find minimal k such that n*k is a binary antipalindrome. */
    for (idx_t k = 1;; ++k)
    {
        seq_t prod = (seq_t)n * k;
        if (is_antipalindrome(prod))
            return (seq_t)k;
    }
}
