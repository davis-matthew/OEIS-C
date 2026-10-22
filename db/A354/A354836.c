/*
 * A354836  Triangle T(n,k) where, if n-k and n+k are prime, T(n,k) = n+k is
 *          the greater term of a Goldbach partition of 2n into two odd
 *          primes, or zero otherwise.
 * https://oeis.org/A354836
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (is_prime(r-k) && is_prime(r+k)) ? r+k : 0 where offset
 *            n≥3 is mapped to triangle coordinates (r,k): m = n-3, t =
 *            floor((sqrt(8*m+1)-1)/2)+1, r = t+2, k = m - t*(t-1)/2. T(r,k) =
 *            r+k if r-k and r+k are odd primes, else 0
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Integer square root for unsigned __int128.  Returns ⌊√x⌋. */
static idx_t isqrt_u128(unsigned __int128 n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* find high such that high*high > n */
    while ((unsigned __int128)high * high <= n) {
        high <<= 1;
    }
    low = high >> 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute a(n) for n ≥ 3 according to the triangle definition. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* m is the zero‑based index within the flattened triangle */
    idx_t m = n - 3;

    /* Compute t = floor((sqrt(8*m+1)-1)/2)+1 */
    unsigned __int128 temp = (unsigned __int128)8 * m + 1;
    idx_t u = isqrt_u128(temp);
    idx_t t = (u - 1) / 2 + 1;          /* t = r - 2 */
    idx_t r = t + 2;                    /* row number (n in T(n,k)) */

    /* Previous total entries before row r */
    unsigned __int128 prod = (unsigned __int128)t * (t - 1) / 2;
    idx_t prev = (idx_t)prod;

    /* Column index k in the row (0‑based) */
    idx_t k = m - prev;

    /* Check primality of r‑k and r+k (both odd primes) */
    if (is_prime(r - k) && is_prime(r + k))
        return (seq_t)(r + k);

    return (seq_t)0;
}
