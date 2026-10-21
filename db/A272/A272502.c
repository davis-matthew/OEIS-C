/*
 * A272502  Number of n-letter strings over a ten-letter alphabet where no
 *          letter appears exactly four times.
 * https://oeis.org/A272502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/4⌋} (-1)^k * C(10,k) * * (∏_{i=0}^{k-1}
 *            C(n-4i,4)) * (10-k)^(n-4k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by inclusion–exclusion on the
 * letters that appear exactly four times.   */
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;      /* sum of terms with even k   */
    seq_t neg = 0;      /* sum of terms with odd  k   */

    /* k cannot exceed 10 or ⌊n/4⌋.  */
    idx_t max_k = n / 4;
    if (max_k > 10) max_k = 10;

    for (idx_t k = 0; k <= max_k; ++k) {
        /* C(10, k) – choose the letters that are forced to have
         * exactly four occurrences.                                 */
        seq_t term_letters = binom(10, k);

        /* Product of C(n-4i, 4) for i = 0 … k-1:
         *   positions for the 4 occurrences of each chosen letter.
         */
        seq_t term_mult = 1;
        for (idx_t i = 0; i < k; ++i) {
            term_mult *= binom(n - 4 * i, 4);
        }

        /* Remaining positions (n-4k) can be filled with any of the
         * remaining 10-k letters.                                        */
        seq_t term_pow = seq_pow((seq_t)(10 - k), n - 4 * k);

        /* Full term for this k.   */
        seq_t term = term_letters * term_mult * term_pow;

        /* Accumulate with the appropriate sign.  */
        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }

    /* The net result is always positive (a(n) >= 0).  */
    return pos - neg;
}
