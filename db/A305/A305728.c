/*
 * A305728  Numbers of the form 216*p^3, where p is a Pythagorean prime
 *          (A002144).
 * https://oeis.org/A305728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 216 * p_n^3, where p_n is the n-th Pythagorean prime (a
 *            prime ≡ 1 (mod 4))
 * Range:     u8 none, u16 n <= 1, u32 n <= 26, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   _floor_isqrt_u64, _prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1 – return 0. */
    if (n == 0) return 0;

    /* Upper bound for the n‑th (ordinary) prime, then double it to be
       comfortably over the n‑th Pythagorean prime. */
    idx_t limit = _prime_limit_for_idx(n);
    if (limit < 10) limit = 10;
    limit *= 2;

    while (1) {
        /* Allocate sieve array: 0 = potential prime, 1 = composite. */
        bool *isComp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
        if (!isComp) return 0;   /* allocation failed */

        /* 0 and 1 are not prime. */
        if (limit >= 0) isComp[0] = true;
        if (limit >= 1) isComp[1] = true;

        /* Eratosthenes sieve. */
        idx_t sq = _floor_isqrt_u64(limit);
        for (idx_t i = 2; i <= sq; ++i) {
            if (!isComp[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    isComp[j] = true;
            }
        }

        /* Find the n‑th prime that is 1 mod 4. */
        idx_t count = 0;
        for (idx_t p = 2; p <= limit; ++p) {
            if (!isComp[p] && (p & 3) == 1) {    /* p % 4 == 1 */
                ++count;
                if (count == n) {
                    seq_t pp = (seq_t)p;
                    pp = pp * pp * pp;          /* p³ */
                    seq_t result = pp * (seq_t)216; /* 216·p³ */
                    free(isComp);
                    return result;
                }
            }
        }

        /* Not enough primes – double the sieve size and try again. */
        free(isComp);
        limit *= 2;
    }
}
