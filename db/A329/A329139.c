/*
 * A329139  Numbers whose prime signature is an aperiodic word.
 * https://oeis.org/A329139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   nth element computed by brute‑force search of numbers whose
 *            prime signature is an aperiodic word
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   factor_exponents, is_aperiodic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_exponents.h"
#include "../utils/is_aperiodic.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but avoid underflow */

    uint64_t target = (uint64_t)n;
    uint64_t count  = 0;
    uint64_t m      = 1;

    const int MAX_EXP = 64; /* enough for any 64‑bit integer */
    int exps[MAX_EXP];

    for (;; ++m) {
        int k = factor_exponents(m, exps);
        if (is_aperiodic(exps, k)) {
            ++count;
            if (count == target)
                return (seq_t)m;
        }
    }
}
