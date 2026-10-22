/*
 * A337633  Triangle read by rows: T(n,k) is the number of nonnegative
 *          integers m < n such that m^k + m == 0 (mod n), where 0 <= k < n.
 * https://oeis.org/A337633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9157, u16 n <= 9157, u32 n <= 9157, u64 n <= 9157,
 *            u128 n <= 9157, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th entry of the triangle T(n,k) read by rows.
   Row r has indices 0 .. r-1.
   Since the first r(r+1)/2 entries cover rows 1..r,
   r is the unique integer with r(r-1)/2 < n ≤ r(r+1)/2.
   The column is k = n - r(r-1)/2 - 1.   */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* modular exponentiation for uint64_t values */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t cur = base % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            res = (unsigned __int128)res * cur % mod;
        cur = (unsigned __int128)cur * cur % mod;
        exp >>= 1ULL;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* determine the row r */
    idx_t r = (idx_t)((sqrt((double)(1ULL + 8ULL * n)) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n) r++;

    /* column k inside the row */
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev - 1;

    /* count solutions m < r to m^k + m ≡ 0 (mod r) */
    seq_t count = 0;
    for (idx_t m = 0; m < r; ++m) {
        uint64_t powmod = mod_pow_uint64((uint64_t)m, (uint64_t)k, (uint64_t)r);
        if ((powmod + m) % r == 0)
            ++count;
    }
    return count;
}
