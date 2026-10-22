/*
 * A337922  Numbers k such that when the first digit of k is shifted to the
 *          end the result is 3*k/2.
 * https://oeis.org/A337922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by concatenating a 16‑digit base pattern *
 *            derived from the decimal expansion of (2(n mod 5)+2)/17. * The
 *            base pattern depends on n as follows: * pattern 0 : digits of
 *            2/17 = 1176470588235294 * pattern 1 : digits of 4/17 =
 *            2352941176470588 * pattern 2 : digits of 6/17 = 352941176...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 5, u128 n <= 10, big any n
 * Helpers:   make_pattern, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"
#include "../utils/make_pattern.h"

/* Return a(n) for the given 1‑based index n */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t patterns[5];
    static seq_t pow10_16;

    if (!initialized) {
        /* Numerators corresponding to the 5 patterns */
        const uint64_t numerators[5] = {2, 4, 6, 8, 10};
        for (int i = 0; i < 5; ++i)
            patterns[i] = make_pattern(numerators[i]);

        /* 10^16 needed for concatenation */
        pow10_16 = pow_seq((seq_t)10, 16);
        initialized = true;
    }

    /* 0‑based index */
    idx_t idx = n - 1;
    unsigned pat_idx = (unsigned)(idx % 5);
    idx_t r = idx / 5 + 1;          /* repetitions, ≥1 */

    seq_t result = patterns[pat_idx];
    for (idx_t k = 1; k < r; ++k)
        result = result * pow10_16 + patterns[pat_idx];

    return result;
}
