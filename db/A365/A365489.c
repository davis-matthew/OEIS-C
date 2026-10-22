/*
 * A365489  The number of divisors of the smallest cube divisible by n.
 * https://oeis.org/A365489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (3*ceil(e/3) + 1) – number of divisors of the
 *            smallest cube * divisible by n. (Equivalent to
 *            A000005(A053149(n)))
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* OEIS offset: a(1) is the first term */
    uint64_t n64 = (uint64_t)n;

    /* maximum distinct prime factors for a 64‑bit integer is 15;
       use 20 to be safe for the static arrays. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64(n64, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t ceil_e3 = (e + 2) / 3;          /* ceil(e/3) */
        uint64_t factor = 3 * ceil_e3 + 1;       /* 3*ceil(e/3)+1 */
        res *= (seq_t)factor;
    }
    return res;
}
