/*
 * A379896  a(n) is the least number k whose digit sums are 2*n-1, 2*n and
 *          2*n+1 in bases 2*n-1, 2*n and 2*n+1 respectively.
 * https://oeis.org/A379896
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 276, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least k such that the digit sums of k in bases
   2n-1, 2n, and 2n+1 are 2n-1, 2n, and 2n+1 respectively.
   The algorithm uses the congruence k ≡ 1 (mod L) where
   L = lcm(2n-2, 2n-1, 2n).  It increments k by L until all three
   digit‑sum conditions are met. */

#include <stdbool.h>
#include "../utils/gcd.h"

static inline seq_t digit_sum_base(seq_t k, seq_t base)
{
    seq_t sum = 0;
    while (k > 0)
    {
        sum += k % base;
        k /= base;
    }
    return sum;
}

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    return a / gcd(a, b) * b;
}

/* Compute a(n) for n ≥ 2. Undefined behaviour if result does not fit in seq_t. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;

    /* Bases: b = 2n-1, b+1 = 2n, b+2 = 2n+1  */
    seq_t b = nn * 2u - 1u;
    seq_t base1 = b;          /* 2n-1 */
    seq_t base2 = b + 1u;     /* 2n */
    seq_t base3 = b + 2u;     /* 2n+1 */

    /* Modulus L = lcm(2n-2, 2n-1, 2n)  */
    seq_t d1 = nn * 2u - 2u;  /* 2n-2 */
    seq_t d2 = nn * 2u - 1u;  /* 2n-1 */
    seq_t d3 = nn * 2u;       /* 2n   */

    seq_t L = 1;
    L = lcm_seq(L, d1);
    L = lcm_seq(L, d2);
    L = lcm_seq(L, d3);

    /* Search k = 1 + t*L, t >= 1, until all digit sums match. */
    for (seq_t t = 1; ; ++t)
    {
        seq_t k = 1 + t * L;
        if (digit_sum_base(k, base1) == base1 &&
            digit_sum_base(k, base2) == base2 &&
            digit_sum_base(k, base3) == base3)
            return k;
    }
}
