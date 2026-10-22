/*
 * A317204  Expansion of n in the p-system based on convergents to sqrt(2).
 * https://oeis.org/A317204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal digits d_k … d_1 where n = Σ d_i * P_i P_i are
 *            Pell numbers (P_1 = 1, P_2 = 2, P_i = 2*P_{i-1} + P_{i-2}),
 *            digits are chosen greedily: for each i from the largest Pell ≤
 *            the current remainder, d_i = floor(remainder / P_i)
 * Range:     u8 n <= 11, u16 n <= 69, u32 n <= 5740, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for UINT64_MAX */

seq_t a(idx_t n)
{
    /* n == 0 is the only case with a leading 0 */
    if (n == 0) return 0;

    /* Build Pell numbers up to the largest one ≤ n */
    /* 128 is more than enough for any n that fits in idx_t */
    static idx_t pell[128];
    size_t len = 0;
    pell[len++] = 1;          /* P_1 = 1 */
    if (n >= 2) {
        pell[len++] = 2;      /* P_2 = 2 */
    }
    while (len + 1 < sizeof(pell)/sizeof(pell[0])) {
        idx_t prev1 = pell[len - 1];
        idx_t prev2 = pell[len - 2];
        unsigned __int128 next = (unsigned __int128)prev1 * 2 + prev2;
        if (next > n || next > (unsigned __int128)UINT64_MAX) {
            break;           /* next Pell number would exceed n or overflow */
        }
        pell[len++] = (idx_t)next;
    }

    /* Greedy decomposition */
    seq_t result = 0;
    idx_t rest = n;
    for (size_t idx = len; idx > 0; --idx) {
        idx_t p = pell[idx - 1];
        idx_t digit = rest / p;        /* 0, 1 or 2 (for positions other than the last) */
        result = result * 10 + (seq_t)digit;
        rest -= digit * p;
    }

    return result;
}
