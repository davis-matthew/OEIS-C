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
 * Helpers:   is_antipalindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_antipalindrome.h"

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
