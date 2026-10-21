/*
 * A284061  Triangle read by rows: T(n,k) = pi(prime(k) * prime(n+1)).
 * https://oeis.org/A284061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2144, u32 n <= 9219, u64 n <= 9219,
 *            u128 n <= 9219, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A284061: a(n) = pi(prime(k) * prime(r+1)),
    where r = smallest integer with r(r+1)/2 >= n
    and k = n - (r-1)r/2.
*/

#include <stdint.h>
#include <math.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

static inline idx_t __row_for_n(idx_t n)
{
    // Initial estimate via the quadratic formula
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    // Correct the estimate
    while ((__uint128_t)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (__uint128_t)(r - 1) * r / 2 >= n) --r;
    return r;
}

static inline idx_t __k_for_n(idx_t n, idx_t r)
{
    idx_t prev_rows = (r - 1) * r / 2;
    return n - prev_rows;
}

seq_t a(idx_t n)
{
    /* a(1) = 3, a(2) = 4, a(3) = 6, a(4) = 6 ... */

    if (n == 0) return 0;   // offset 1 – not used, safe guard

    idx_t r = __row_for_n(n);
    idx_t k = __k_for_n(n, r);

    uint32_t prime_k    = get_prime((idx_t)k);
    uint32_t prime_np1  = get_prime(r + 1);

    seq_t prod = (seq_t)prime_k * (seq_t)prime_np1;

    return compute_pi((idx_t)prod);
}
