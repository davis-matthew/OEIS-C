/*
 * A350169  Write 1st prime and decrement 0 times, then write 2nd prime and
 *          decrement once, write 3rd prime and decrement twice, write 4th
 *          prime and decrement 3 times, etc ...
 * https://oeis.org/A350169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A350169
 *  a(n) is the element obtained by writing the r‑th prime p_r
 *  and then decreasing it by 1, r times.  The row containing
 *  a(n) is the smallest r such that r*(r+1)/2 ≥ n.
 *  a(n) = p_r - (n - T(r-1) - 1), where T(k)=k*(k+1)/2.  */

#include "../utils/get_prime.h"

/* Computes a(n),  n ≥ 1  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n<1 */

    /* Find r with T(r-1) < n ≤ T(r)   (binary search) */
    idx_t lo = 1, hi = 2;
    /* grow hi until enough */
    while (((unsigned __int128)hi * (hi + 1) / 2) < n)
        hi <<= 1U;

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                    /* row index (1‑based) */

    /* start index of row r (0‑based) = T(r-1) */
    unsigned __int128 start128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t start = (idx_t)start128;

    /* column offset inside the row (0‑based) */
    uint64_t i = n - start - 1;

    /* r‑th prime */
    uint32_t p = get_prime(r);

    /* result */
    return (seq_t)p - (seq_t)i;
}
