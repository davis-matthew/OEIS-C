/*
 * A283533  a(n) = Sum_{d|n} d^(2*d + 1).
 * https://oeis.org/A283533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(2*d + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive helper to enumerate all divisors and accumulate the sum. */
static void rec_divs(size_t idx, size_t cnt, const uint64_t primes[],
                     const uint64_t exps[], seq_t curr, seq_t *sum)
{
    if (idx == cnt) {
        seq_t base = curr;
        seq_t exp  = (seq_t)2 * base + 1;      /* exponent = 2*base + 1 */
        seq_t pw   = big_pow(base, exp);       /* base^exp */
        *sum += pw;
        return;
    }

    seq_t cur = curr;
    for (size_t k = 0; k <= exps[idx]; ++k) {
        rec_divs(idx + 1, cnt, primes, exps, cur, sum);
        cur *= (seq_t)primes[idx];              /* multiply by next power of prime */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined but safe fallback */

    /* Factor n.  The number of prime factors for n <= 10000 is ≤ 16. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    rec_divs(0, cnt, primes, exps, (seq_t)1, &sum);
    return sum;
}
