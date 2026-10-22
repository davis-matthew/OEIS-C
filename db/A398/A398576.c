/*
 * A398576  a(n) = A008836(A000026(n)).
 * https://oeis.org/A398576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} -A008836(e) * where A008836 is the Liouville
 *            λ‑function. Thus a is multiplicative and * for every prime power
 *            p^e in n * a(p^e) = -λ(e) * λ(e) = (-1)^{Ω(e)} (Ω = total number
 *            of prime factors counted with multiplicity). * * The
 *            implementation factors n, then for each exponent e *...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1; but we handle 0 defensively. */
    if (n == 0) return 1;

    /* factor_u64 returns at most 64 distinct prime factors for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;                 /* ±1 during the loop */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];

        /* compute parity of Ω(e): number of prime factors of e (with multiplicity) */
        int parity = 0;               /* 0 = even, 1 = odd */
        uint64_t x = e;
        for (uint64_t d = 2; d * d <= x; ++d) {
            while (x % d == 0) {
                parity ^= 1;          /* flip parity for each factor */
                x /= d;
            }
        }
        if (x > 1) parity ^= 1;       /* remaining prime factor */

        /* Ω(e) even → λ(e)=1 → -λ(e) = -1 → flip sign of result */
        if ((parity & 1) == 0) {
            result = -result;
        }
        /* Ω(e) odd → λ(e)=-1 → -λ(e)=+1 → no change */
    }

    return result;
}
