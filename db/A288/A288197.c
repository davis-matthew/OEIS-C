/*
 * A288197  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 430", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(2)=0; for odd n (n%2==1 or n==0): a(n)=2^n; for even
 *            n≥4: let m=floor_log2(n+1); let k = 2^(m-1)-1; a(n)=2^n -
 *            2^(n-k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 2) {
        return 0;
    }
    if ((n & 1) || n == 0) {
        return pow2(n);
    }
    idx_t m = floor_log2_uint64(n + 1);
    idx_t prev = ((idx_t)1 << (m - 1));
    idx_t k = prev - 1;
    idx_t exp = n - k;
    seq_t pow_n = pow2(n);
    seq_t pow_sub = pow2(exp);
    return pow_n - pow_sub;
}
