/*
 * A244541  Numbers n such that n +/- the product of digits of n are both
 *          palindromes.
 * https://oeis.org/A244541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function returns the nth member of A244541 by brute‑force
 *            search
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_product, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_product.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t cand   = 1;          /* candidate number */
    idx_t found  = 0;          /* how many valid terms seen */

    while (1) {
        seq_t prod = digit_product(cand);

        /* cand - prod must not underflow */
        if (cand >= prod) {
            seq_t plus  = cand + prod;
            seq_t minus = cand - prod;

            if (is_palindrome(plus, 10) && is_palindrome(minus, 10)) {
                ++found;
                if (found == n)
                    return cand;
            }
        }
        ++cand;
    }
}
