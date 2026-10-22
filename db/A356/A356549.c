/*
 * A356549  a(n) is the number of divisors of 10^n whose first digit is 1.
 * https://oeis.org/A356549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of 10^n whose first digit is 1
 * Range:     u8 n <= 28, u16 n <= 465, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the observation that a divisor of 10^n is 2^i5^j
 * (0 ≤ i,j ≤ n).  The condition that 2^i5^j starts with 1
 * depends only on the difference d = i – j.  For a given d, the number
 * of pairs (i,j) with that difference is n – |d| + 1.  The fractional
 * part of d·log10(2) determines whether the leading digit is 1.  We
 * compute that fractional part with 18‑digit fixed‑point arithmetic
 * using unsigned __int128.  The algorithm runs in O(n) time. */

#include <stdint.h>

/* Fixed‑point representation of log10(2) with 18 fractional digits */
static const uint64_t LOG2P = 301029995663981195ULL;           /* floor(log10(2) * 10^18) */
static const uint64_t U     = 1000000000000000000ULL;         /* 10^18  */
static const uint64_t THR_N = U - LOG2P;                      /* U - LOG2P */

seq_t a(idx_t n)
{
    /* a(0) = 1 : the divisor 1 */
    seq_t total = (seq_t)(n + 1);      /* contribution from i = j */

    /* For each non‑zero difference d, compute weight and test the
     * leading‑digit condition.  The condition depends on integer
     * arithmetic only. */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t weight = (seq_t)(n - d + 1);          /* pairs with this |d| */

        /* f_mod = floor(d * LOG2P) mod U, i.e. the fractional part */
        unsigned __int128 prod  = (unsigned __int128)d * LOG2P;
        uint64_t f_mod = (uint64_t)(prod % U);

        /* positive difference case (i > j) */
        if (f_mod < LOG2P) {
            total += weight;
        }

        /* negative difference case (i < j) */
        if (f_mod == 0 || f_mod > THR_N) {
            total += weight;
        }
    }
    return total;
}
