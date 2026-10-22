/*
 * A324600  a(n) = (k(n)*(k(n) + 1))/2 with k = A018252 (nonprime numbers),
 *          for n >= 1.
 * https://oeis.org/A324600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(k(n)) = k(n)*(k(n)+1)/2 , where k(n) is the n‑th *
 *            nonprime (composite or 1). For n ≥ 1, a(1) = 1, a(2) = 10, …
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* offset handling – unused for valid n≥1 */
        return 0;

    idx_t k = 0;               /* candidate integer */
    idx_t count = 0;           /* how many nonprimes seen */

    while (count < n) {
        ++k;
        if (!is_prime(k))      /* nonprime found */
            ++count;
    }

    seq_t K = (seq_t)k;        /* cast to seq_t first to avoid overflow */
    return (K * (K + 1)) / 2;
}
