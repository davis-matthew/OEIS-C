/*
 * A285320  If n == 0 or A008683(n) == 0, then a(n) = 0, otherwise a(n) =
 *          1+a(A048675(n)); number of iterations of A048675 needed before the
 *          result is either zero or nonsquarefree number (A013929).
 * https://oeis.org/A285320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = 0 if n = 0 or n is not squarefree, otherwise a(n) = 1 +
 *            a(A048675(n)); computed by iterating A048675 (via trial division
 *            plus factor_u64) until a nonsquarefree number is reached
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 73 (2026-11-08)
 * Helpers:   factor_u64, pow2, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"
#include "../utils/prime_pi.h"

/* returns 1 if m squarefree (out = A048675(m)), 0 if not squarefree, -1 if unknown */
static int A285320_step(seq_t m, seq_t *out)
{
    seq_t sum = 0;
    seq_t rem = m;
    for (unsigned long long p = 2; p <= 65536ULL; ++p) {
        if (rem == 1) break;
        if ((unsigned long long)p * p > (unsigned long long)65536ULL * 65536ULL) break;
        if (rem % (seq_t)p == 0) {
            rem /= (seq_t)p;
            if (rem % (seq_t)p == 0) return 0;
            sum += pow2((idx_t)prime_pi((idx_t)p) - 1);
        }
    }
    if (rem > 1) {
        if (rem / 9223372036854775808ULL != 0) return -1;
        uint64_t r = (uint64_t)(unsigned long long)rem;
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(r, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] > 1) return 0;
            sum += pow2((idx_t)prime_pi((idx_t)primes[i]) - 1);
        }
    }
    *out = sum;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t m = (seq_t)n;
    seq_t count = 0;
    while (m != 0) {
        seq_t nxt = 0;
        int r = A285320_step(m, &nxt);
        if (r <= 0) break;
        ++count;
        m = nxt;
    }
    return count;
}
