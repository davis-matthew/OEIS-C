/*
 * A349890  Triangle read by rows: T(n,k) = n * 2^e(n) - (4^e(n) - 1) / 3 - k
 *          * (k - 1) / 2 with e(n) = 1 + floor(log_2(n)) for n >= 1 and 1 <=
 *          k <= n.
 * https://oeis.org/A349890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) enumerates the triangle * T(r,k) = r*2^e(r) - (4^e(r)-1)/3
 *            - k*(k-1)/2, * where e(r) = 1 + floor(log₂ r), * r is the
 *            smallest integer with r(r+1)/2 ≥ n, * k = n - r(r-1)/2. This
 *            produces the permutation given * in the OEIS entry
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety; sequence starts at n = 1 */

    /* find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    __uint128_t tri;
    while (1) {
        tri = (__uint128_t)r * (r + 1) / 2;
        if (tri >= n) break;
        r++;
    }

    /* column k within that row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    /* compute e(r) = 1 + floor(log₂ r) */
    unsigned int l = 0;
    idx_t tmp = r;
    while (tmp > 1) {          /* count bits in r */
        tmp >>= 1;
        l++;
    }
    unsigned int e = l + 1;

    /* powers of 2 */
    seq_t pow2_e  = ((seq_t)1) << e;
    seq_t pow4_e  = pow2_e * pow2_e;   /* 4^e */

    /* compute T(r,k) */
    seq_t term = ((seq_t)r) * pow2_e
               - ((pow4_e - (seq_t)1) / (seq_t)3)
               - (((seq_t)k * (k - 1)) / (seq_t)2);
    return term;
}
