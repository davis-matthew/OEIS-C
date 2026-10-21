/*
 * A253228  Number of (n+1) X (5+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minimum minus antidiagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal maximum
 *          nondecreasing vertically.
 * https://oeis.org/A253228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/32)*(5280 + 697*2^n + 1840*n + 208*n^2) for n>4
 *            a(1)=907, a(2)=1775, a(3)=1709, a(4)=2543
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 73,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 1) return (seq_t)907;
    if (n == 2) return (seq_t)1775;
    if (n == 3) return (seq_t)1709;
    if (n == 4) return (seq_t)2543;

    /* term1 = ((5280 + 1840*n + 208*n*n) * 3) / 32 */
    unsigned long long sum = 5280ULL
                          + 1840ULL * n
                          + 208ULL * n * n;
    unsigned long long term1 = sum * 3ULL / 32ULL;
    seq_t term1_val = (seq_t)term1;

    idx_t shift = n - 5;
#ifdef __cplusplus
    if constexpr (std::is_unsigned<seq_t>::value && sizeof(seq_t) <= 8) {
        /* Built‑in unsigned integer (≤64 bits) */
        seq_t powshift = (shift < 64) ? (seq_t)1 << shift : (seq_t)0;
        seq_t term2 = 0;
        if (shift <= 52) {              /* keep multiplication in range */
            term2 = powshift * (seq_t)2091;
        }
        return term1_val + term2;
    } else {
        /* Arbitrary‑precision type */
        seq_t powshift = seq_pow((seq_t)2, shift);
        seq_t term2 = powshift * (seq_t)2091;
        return term1_val + term2;
    }
#else
    /* C: assume seq_t is an unsigned integer of 64‑bits or smaller */
    seq_t powshift = (shift < 64) ? (seq_t)1 << shift : (seq_t)0;
    seq_t term2 = 0;
    if (shift <= 52) {
        term2 = powshift * (seq_t)2091;
    }
    return term1_val + term2;
#endif
}
