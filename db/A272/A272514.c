/*
 * A272514  Number of set partitions of [n] into two blocks with distinct
 *          sizes.
 * https://oeis.org/A272514
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} binomial(n,i)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 968 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3u) {
        return 0; /* undefined for n < 3, offset starts at 3 */
    }
    seq_t power = pow2(n - 1);          /* 2^(n-1) */
    if (n & 1u) {                       /* n odd */
        return power - 1;
    } else {                            /* n even */
        seq_t central = binomial(n, n / 2); /* C(n, n/2) */
        seq_t half = central / 2;          /* divide by 2 */
        return power - half - 1;
    }
}
