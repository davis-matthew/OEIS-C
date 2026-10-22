/*
 * A321856  Number of primes of the form 3*m + 2 <= n minus number of primes
 *          of the form 3*m + 1 <= n.
 * https://oeis.org/A321856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = #{primes ≤ n, p ≡ 2 (mod 3)} – #{primes ≤ n, p ≡ 1 (mod
 *            3)}
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime_small_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small_local.h"

seq_t a(idx_t n)
{
    if (n < 2u) return 0;

    seq_t diff = 0;
    for (idx_t p = 2u; p <= n; ++p)
    {
        if (!is_prime_small_local(p)) continue;
        if (p == 3u) continue;                 /* 3 contributes 0 */
        if (p % 3u == 2u)
            ++diff;
        else if (p % 3u == 1u)
            --diff;
    }
    return diff;
}
