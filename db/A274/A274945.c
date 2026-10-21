/*
 * A274945  The Staircase Sequence: numbers with at least three digits and
 *          with the property that the sum of the squares of the first and
 *          last digits equals the number obtained when the first and last
 *          digits are deleted.
 * https://oeis.org/A274945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 143, u32 n <= 573, u64 n <= 1443,
 *            u128 n <= 3173, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A274945:  the Staircase Sequence.
   For n ≤ 83 it does a brute‑force search for the nth term,
   for n > 83 it uses the explicit formula given in the OEIS entry. */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static bool is_staircase(seq_t x)
{
    /* number of decimal digits */
    int d = digit_len(x);
    if (d < 3)
        return false;

    seq_t pow_d1 = pow10((idx_t)(d - 1));
    seq_t first  = x / pow_d1;                     /* most significant digit */
    seq_t pow_d2 = pow10((idx_t)(d - 2));
    seq_t middle = (x / 10) % pow_d2;              /* inner part */
    seq_t last   = x % 10;                         /* least significant digit */

    seq_t chk = first * first + last * last;
    return chk == middle;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset starts at 1; n==0 never requested */

    if (n <= 83) {
        /* brute‑force generation of the first 83 terms */
        seq_t cand = 110;
        idx_t found = 0;
        for (;;) {
            if (is_staircase(cand)) {
                ++found;
                if (found == n)
                    return cand;
            }
            ++cand;
        }
    }

    /* explicit formula for n > 83 (OEIS A274945) */
    idx_t D = (n + 366) / 90;                     /* number of digits of a(n) */
    idx_t i = 1 + (n + 366) / 10 - 9 * D;         /* first digit   */
    idx_t j = (n - 4) % 10;                       /* last  digit   */

    seq_t pow_ = pow10(D - 1);
    seq_t term = (seq_t)i * pow_ + ((seq_t)(i * i + j * j) * 10 + j);
    return term;
}
