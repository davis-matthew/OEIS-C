/*
 * A283799  Number of dispersed Dyck prefixes of length 2n and height n.
 * https://oeis.org/A283799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = binomial(2n, floor(n/2)) + binomial(2n, ceil(n/2)-1)
 *  (for n >= 0, a(0)=1)
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Generic binomial helper.  Two specialisations:
 *   - integral seq_t's use 128‑bit arithmetic to avoid overflow
 *   - non‑integral seq_t's (big integers) use seq_t operations
 */
static inline seq_t binomial_impl(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    /* special case: k == 0 */
    if (k == 0) return 1;

#ifdef __cplusplus
    /* C++ build     (seq_t may be a big integer class) */
    if (!std::is_integral<seq_t>::value) {
        seq_t res = 1;
        for (idx_t i = 1; i <= k; ++i) {
            /* res = res * (n-k+i) / i ; all exact */
            res = (res * static_cast<seq_t>(n - k + i)) / static_cast<seq_t>(i);
        }
        return res;
    }
#endif

    /* C build or integral seq_t (uint*_t) */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)res * (n - k + i);
        res = (seq_t)(tmp / i);
    }
    return res;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

    /* ceil(n/2) */
    idx_t k = (n + 1) / 2;

    seq_t part1 = binomial_impl(2 * n, n - k);   /* binomial(2n, floor(n/2)) */
    seq_t part2 = binomial_impl(2 * n, k - 1);   /* binomial(2n, ceil(n/2)-1) */

    return part1 + part2;
}
