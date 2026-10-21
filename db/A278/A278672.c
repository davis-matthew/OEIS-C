/*
 * A278672  Number of nX4 0..1 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly one mistake.
 * https://oeis.org/A278672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{20} Δ^k a(1) * binomial(n-1, k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 68, u128 n <= 210,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Δ^k a(1) are the forward differences of the first 21 terms. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* First 21 terms from the OEIS entry. */
    static const uint64_t init_terms[21] = {
        10ULL, 99ULL, 579ULL, 2650ULL, 10584ULL, 38848ULL,
        134265ULL, 441349ULL, 1384443ULL, 4148373ULL,
        11882640ULL, 32576006ULL, 85619948ULL, 216195769ULL,
        525654868ULL, 1233512143ULL, 2800037121ULL,
        6161774705ULL, 13172137674ULL, 27405600826ULL,
        55592487870ULL
    };

    /* Forward differences Δ^k a(1) for k = 0..20. */
    static seq_t diff[21];
    static bool initialized = false;

    if (!initialized) {
        uint64_t arr[21];
        for (int i = 0; i < 21; ++i)
            arr[i] = init_terms[i];

        for (int k = 0; k <= 20; ++k) {
            diff[k] = (seq_t)arr[0];
            for (int i = 0; i < 20 - k; ++i)
                arr[i] = arr[i + 1] - arr[i];
        }
        initialized = true;
    }

    seq_t result = 0;
    for (int k = 0; k <= 20; ++k) {
        seq_t bin = binomial(n - 1, (idx_t)k);  /* C(n-1, k) */
        result += diff[k] * bin;
    }
    return result;
}
