/*
 * A354527  a(n) = A001221(n) - A354512(n).
 * https://oeis.org/A354527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{p | p is a distinct prime factor of n and gpf(n + p) !=
 *            p}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gpf.h"

/*
 * Computes a(n) for the OEIS sequence A354527.
 *
 * a(n) = number of distinct prime factors p of n such that the greatest
 *        prime factor of (n + p) is different from p.
 *
 * The function uses only the supplied gpf() routine and a simple
 * trial‑division factorisation of n.  It works for all n ≥ 1.
 *
 * It is efficient for the intended range (n up to about 10 000) and
 * does not rely on any global state.
 */
seq_t a(idx_t n)
{
    /* Get the distinct prime factors of n */
    uint64_t remaining = (uint64_t)n;
    uint64_t primes[32];
    size_t pf_cnt = 0;

    if (remaining % 2 == 0) {
        primes[pf_cnt++] = 2;
        while (remaining % 2 == 0) remaining /= 2;
    }

    for (uint64_t d = 3; d * d <= remaining; d += 2) {
        if (remaining % d == 0) {
            primes[pf_cnt++] = d;
            while (remaining % d == 0) remaining /= d;
        }
    }

    if (remaining > 1) {
        primes[pf_cnt++] = remaining;
    }

    /* Count the primes that fail the gpf test */
    seq_t result = 0;                    /* seq_t is unsigned */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t val = (uint64_t)n + p;
        if (gpf(val) != p) {
            result++;
        }
    }
    return result;
}
