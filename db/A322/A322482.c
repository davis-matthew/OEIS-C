/*
 * A322482  Table read by downward antidiagonals: T(n,k) is the greatest
 *          divisor of n which is a unitary divisor of k.
 * https://oeis.org/A322482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry read by downward antidiagonals of the
 *            table T(n,k) with T(n,k) = max{ d : d|n, d|k, gcd(d,k/d)=1 }
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* returns the n-th term of OEIS A322482 */
seq_t a(idx_t n)
{
    /* 1. Find the diagonal d such that the cumulative count up to d
       (i.e. (d-1)d/2) is at least n.  Use binary search on 64‑bit
       unsigned integers; overflow is avoided with 128‑bit arithmetic. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)(high - 1) * high / 2 < n) high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cum = (unsigned __int128)(mid - 1) * mid / 2;
        if (cum >= n) high = mid;
        else low = mid + 1;
    }
    idx_t d = low;  /* diagonal index (sum of the coordinates) */

    /* 2. Position of (i,j) inside this diagonal */
    unsigned __int128 cumPrev = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t pos = (idx_t)(n - cumPrev - 1);   /* 0‑based position within diagonal */

    idx_t i = pos + 1;          /* row number */
    idx_t j = d - i;            /* column number */

    /* 3. Compute T(i,j) = product of prime powers of j that divide i */
    seq_t result = 1;
    uint64_t k = (uint64_t)j;   /* mutable copy of j for factorisation */
    uint64_t prime = 2;
    while ((unsigned __int128)prime * prime <= k) {
        if (k % prime == 0) {
            uint64_t pw = 1;
            while (k % prime == 0) {
                k /= prime;
                pw *= prime;
            }
            if ((seq_t)i % pw == 0) {
                result *= pw;
            }
        }
        ++prime;
    }
    if (k > 1) {               /* remaining prime factor */
        if ((seq_t)i % k == 0) {
            result *= k;
        }
    }
    return result;
}
