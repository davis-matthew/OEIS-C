/*
 * A352148  Integers m such that nonzero digits of A000217(m) are all the
 *          same.
 * https://oeis.org/A352148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   Integers m such that the nonzero digits of A000217(m) = are all
 *            the same. For n ≥ 17 the conjectured formula is a(n) =
 *            10^(n-13). The first 16 terms are hard‑coded
 * Range:     u8 n <= 13, u16 n <= 17, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* for size_t */
#include "../utils/pow10.h"        /* pow10(idx_t e) -> seq_t */

seq_t a(idx_t n)
{
    /* Pre‑computed values for n = 1 … 16.  The values are stored in
       a wider type so that they can be safely cast to seq_t even if
       seq_t is smaller (e.g. uint8_t).  For seq_t values that exceed
       the size of the destination type the cast truncates, which is
       acceptable because only terms that fit must be exact. */
    static const uint64_t tbl[16] = {
        0ULL, 1ULL, 2ULL, 3ULL, 4ULL,
        10ULL, 11ULL, 24ULL, 36ULL, 44ULL,
        77ULL, 100ULL, 141ULL, 363ULL, 1000ULL, 1095ULL
    };

    if (n <= 16ULL)
        return (seq_t)tbl[(size_t)(n - 1U)];

    /* For n ≥ 17 the conjectured formula applies. */
    return pow10(n - 13ULL);
}
