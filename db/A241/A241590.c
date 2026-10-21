/*
 * A241590  Numerators of Postnikov's hook-length formula 2^n*(n+1)^(n-1)/n!.
 * https://oeis.org/A241590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 2^n * (n+1)^(n-1) / n!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    idx_t n1 = n + 1;          /* value to be factored */

    /* Factor n+1: distinct primes and their exponents */
    /* n+1 <= 10001 for the required constraint, so 16 slots are safe. */
    const size_t MAX_FACTORS = 16;
    idx_t primes[MAX_FACTORS];
    idx_t exps[MAX_FACTORS];
    size_t pf_count = 0;

    /* factor 2 */
    idx_t cnt = 0;
    while (n1 % 2 == 0) {
        n1 /= 2;
        cnt++;
    }
    if (cnt > 0) {
        primes[pf_count]     = 2;
        exps[pf_count]       = cnt;
        pf_count++;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= n1; p += 2) {
        if (n1 % p == 0) {
            cnt = 0;
            while (n1 % p == 0) {
                n1 /= p;
                cnt++;
            }
            if (pf_count < MAX_FACTORS) {
                primes[pf_count]     = p;
                exps[pf_count]       = cnt;
                pf_count++;
            }
        }
    }
    if (n1 > 1) {          /* remaining prime factor > 1 */
        if (pf_count < MAX_FACTORS) {
            primes[pf_count]   = n1;
            exps[pf_count]     = 1;
            pf_count++;
        }
    }

    seq_t result = (seq_t)1;

    /* handle prime 2 (always present via the 2^n factor) */
    idx_t vp2 = 0;
    for (size_t i = 0; i < pf_count; ++i)
        if (primes[i] == 2) {
            vp2 = exps[i];
            break;
        }

    idx_t expA2 = n + (n - 1) * vp2;
    idx_t expB2 = 0;
    idx_t m = n;
    while (m) {
        m /= 2;
        expB2 += m;
    }
    if (expA2 > expB2) {
        idx_t diff = expA2 - expB2;
        seq_t term = big_pow((seq_t)2, (seq_t)diff);
        result = seq_mul(result, term);
    }

    /* handle odd primes that divide n+1 */
    for (size_t i = 0; i < pf_count; ++i) {
        idx_t p = primes[i];
        if (p == 2) continue;        /* already processed */

        idx_t vp = exps[i];
        idx_t expA = (n - 1) * vp;

        idx_t expB = 0;
        m = n;
        while (m) {
            m /= p;
            expB += m;
        }

        if (expA > expB) {
            idx_t diff = expA - expB;
            seq_t term = big_pow((seq_t)p, (seq_t)diff);
            result = seq_mul(result, term);
        }
    }

    return result;
}
