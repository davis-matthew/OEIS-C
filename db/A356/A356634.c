/*
 * A356634  a(n) = n! * Sum_{k=0..floor(n/4)} (n - 4*k)^k/24^k.
 * https://oeis.org/A356634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (n-4k)^k / 24^k
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t result = 0;
    seq_t denom  = 1;          /* 24^k for current k */
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t base = (seq_t)(n - 4 * k);    /* non‑negative by construction */
        seq_t power = seq_pow(base, k);       /* (n-4k)^k      */
        seq_t term = (fact / denom) * power;  /* n! / 24^k * (n-4k)^k */
        result += term;
        denom *= 24;                          /* advance 24^(k+1)    */
    }
    return result;
}
