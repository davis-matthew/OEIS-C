/*
 * A395541  Largest integer of the form r^k whose product of nonzero base-n
 *          digits is r and whose number of base-n digits is k.
 * https://oeis.org/A395541
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = largest integer of the form r^k such that * P_n(r^k)=r
 *            and r^k has exactly k base-n digits
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digits_and_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search r and k up to a reasonable bound and keep the maximum. */

#include <stdint.h>
#include "../utils/digits_and_product.h"

/* Compute a(n) for n >= 2. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t best = 0;
    const int MAX_K = 40;   /* heuristic upper bound on k */

    for (idx_t r = 1; r < n; ++r)
    {
        seq_t val = (seq_t)r;

        /* r == 1 only works for k = 1 */
        if (r == 1)
        {
            if (val > best)
                best = val;
            continue;
        }

        for (int k = 1; k <= MAX_K; ++k)
        {
            seq_t prod;
            int d = digits_and_product(val, (seq_t)n, &prod);

            if (d == k && prod == (seq_t)r && val > best)
                best = val;

            if (k == MAX_K)
                break;

            val *= (seq_t)r;
        }
    }

    return best;
}
