/*
 * A256530  Number of ON cells at n-th stage in simple 2-dimensional cellular
 *          automaton (see Comments lines for definition).
 * https://oeis.org/A256530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0)=0; for n≥1 let i = ceil(log2(n+1)), pow = 2^(i-1), j = n -
 *            pow. Then a(n) = (2^i-1)^2 + 3*(2*j)^2
 * Range:     u8 n <= 9, u16 n <= 134, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute i = bit-length of n = ceil(log2(n+1)) */
    unsigned long long nn = (unsigned long long)n;
    unsigned int i = 64 - __builtin_clzll(nn);   /* i ≥ 1 for n>0 */

    /* pow = 2^(i-1) as idx_t */
    idx_t pow_idx = (idx_t)1ULL << (i - 1);
    idx_t j = n - pow_idx;                       /* 0 ≤ j < pow_idx */

    /* side = 2^i - 1 ; handle i==64 specially for 64‑bit builds */
    seq_t side;
    if (i == 64) {
        side = (seq_t)-1;            /* 2^64-1 when seq_t is uint64_t */
    } else {
        side = (((seq_t)1) << i) - (seq_t)1;
    }

    seq_t ans = side * side;                 /* (2^i-1)^2 */

    /* add 3*(2*j)^2 == 12*j^2 */
    seq_t jj = (seq_t)j * (seq_t)j;
    ans += (seq_t)12 * jj;

    return ans;
}
