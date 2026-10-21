/*
 * A249067  Smallest k such that {first digit of j*n, 0<=j<=k} = {0,1,...,9}.
 * https://oeis.org/A249067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = smallest k such that the set of first digits of * j*n for
 *            0 <= j <= k equals {0,1,2,3,4,5,6,7,8,9}. Computed by
 *            brute‑force * search with no precomputation
 * Range:     u8 n <= 11, u16 n <= 1926, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 10 possible first digits: 0-9 */
    bool seen[10] = {0};
    int count = 0;
    idx_t j = 0;

    /* j = 0 always contributes the digit 0 */
    seen[0] = true;
    count = 1;
    /* If n==0 (not in domain) return 0 to avoid infinite loop */
    if (n == 0)
        return (seq_t)0;

    for (;; ++j) {
        seq_t prod = (seq_t)j * n;
        /* find first (most significant) digit of prod */
        while (prod >= 10) {
            prod /= 10;
        }
        int d = (int)prod;          /* d in 0..9 */
        if (!seen[d]) {
            seen[d] = true;
            ++count;
            if (count == 10)
                return (seq_t)j;
        }
    }
}
