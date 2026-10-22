/*
 * A353612  Triangle read by rows: T(n,k) = (n + k) if (n + k) is a prime
 *          number, otherwise T(n,k) = 0; n >= 1, k >= 1.
 * https://oeis.org/A353612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r + k) if (r + k) is prime, else 0 * where r = smallest
 *            integer with r(r+1)/2 >= n, * and k = n - r(r-1)/2. * The
 *            triangle T(n,k) = (n+k) if prime else 0, read row‑wise
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>


/* Small primality test for uint64_t (used because r+k is tiny) */
static bool is_prime_small_uint64(uint64_t x)
{
    if (x < 2)            return false;
    if (x <= 3)           return true;
    if ((x & 1ULL) == 0)  return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)   return false;
    return true;
}

seq_t a(idx_t n)
{
    /*  a(1) corresponds to the first entry of the triangle.  */
    if (n == 0) return 0;      /* offset is 1; undefined for n<1 */

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        r++;

    /* Column k (1‑based) within that row */
    idx_t prev = (r - 1) * r / 2;   /* elements before row r */
    idx_t k    = n - prev;          /* 1 <= k <= r */
    idx_t m    = r + k;             /* candidate value */

    /* Return m if it is prime, otherwise 0 */
    if (is_prime_small_uint64((uint64_t)m))
        return (seq_t)m;
    else
        return (seq_t)0;
}
