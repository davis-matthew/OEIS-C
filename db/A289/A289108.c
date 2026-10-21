/*
 * A289108  Triangle read by rows: T(n,k) = (k + 1)*prime(n) + k for n > 0, 0
 *          <= k <= n, and with T(0,0) = 1.
 * https://oeis.org/A289108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (k + 1)*prime(n) + k (for n>0, 0 ≤ k ≤ n); a(n) is
 *            obtained by reading the triangle T row‑by‑row, with T(0,0) = 1
 * Range:     u8 n <= 62, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"   /* Provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)   /* the special first term */
        return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       r is the 1‑based row index used in the formula (prime(r)). */
    idx_t low  = 0;
    idx_t high = 1;

    /* Increase high until the cumulative count exceeds n.
       Using unsigned __int128 to avoid overflow. */
    while (((unsigned __int128)high * (high + 1) / 2) <= n)
    {
        if (high >= (1ULL << 62))   /* safety against infinite loop */
            break;
        high <<= 1;
    }

    /* Binary search for the smallest high with sum > n. */
    while (high > low)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;          /* row index (≥1 for n>0) */

    /* Position within row: k = n - r*(r+1)/2.  */
    idx_t prev_sum = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - prev_sum;     /* 0 ≤ k ≤ r */

    /* Retrieve the r‑th prime (1‑based).  */
    seq_t prime_val = (seq_t)get_prime(r);   /* get_prime returns a uint32_t */

    /* Compute the triangle value: (k+1)*prime + k.  */
    __int128 tmp = (__int128)(k + 1) * (__int128)prime_val
                  + (__int128)k;

    return (seq_t)tmp;   /* cast to the required return type */
}
