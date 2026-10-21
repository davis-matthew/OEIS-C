/*
 * A275806  a(n) = number of distinct nonzero digits in factorial base
 *          representation of n.
 * https://oeis.org/A275806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct nonzero digits in the factorial base
 *            representation of n
 * Range:     u8 n <= 192, u16 n <= 9262, u32 n <= 9262, u64 n <= 9262,
 *            u128 n <= 9262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle the trivial case n == 0 */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;

    /* Find the largest k such that k! ≤ n */
    seq_t fac = 1;            /* 1!  */
    idx_t maxk = 0;
    while (fac <= nn) {
        ++maxk;
        fac *= (seq_t)(maxk + 1);   /* fac becomes (maxk+1)! */
    }

    /* Compute factorials up to maxk */
    seq_t facs[25];                /* 25 > 20 ; maxk ≤ 20 for n ≤ 2^64-1 */
    facs[1] = (seq_t)1;
    for (idx_t i = 2; i <= maxk; ++i) {
        facs[i] = facs[i - 1] * (seq_t)i;
    }

    /* Count distinct non‑zero digits using a bit mask */
    unsigned long long mask = 0;
    for (int i = (int)maxk; i >= 1; --i) {
        seq_t digit = nn / facs[i];
        nn = nn % facs[i];
        if (digit != 0) {
            unsigned long long d = (unsigned long long)digit; /* d ≤ i ≤ maxk ≤ 20 */
            mask |= 1ULL << d;
        }
    }

    /* Popcount of the mask */
    unsigned count = 0;
    unsigned long long m = mask;
    while (m) {
        count += m & 1U;
        m >>= 1;
    }

    return (seq_t)count;
}
