/*
 * A344316  Number of primes appearing along the border of an n X n square
 *          array whose elements are the numbers from 1..n^2, listed in
 *          increasing order by rows.
 * https://oeis.org/A344316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) + pi(n^2-1) - pi(n^2-n) + Sum_{k=1..n-2} (pi(n*k+1)
 *            – pi(n*k)) (OEIS A344316)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/compute_pi.h"   /* provides seq_t compute_pi(idx_t limit); */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1, but return 0 */

    seq_t res = 0;

    /* pi(n) */
    res += compute_pi(n);

    if (n > 1) {
        /* pi(n^2-1) */
        idx_t sq_minus_one = n * n - 1;
        res += compute_pi(sq_minus_one);

        /* pi(n^2-n) */
        idx_t sq_minus_n = n * n - n;
        res -= compute_pi(sq_minus_n);

        /* Sum_{k=1..n-2} (pi(n*k+1) - pi(n*k)) */
        for (idx_t k = 1; k <= n - 2; ++k) {
            idx_t base = n * k;
            res += compute_pi(base + 1) - compute_pi(base);
        }
    }

    return res;
}
