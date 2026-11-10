/*
 * A249810  a(1) = 0, a(n) = A078898(A003961(n)).
 * https://oeis.org/A249810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(n) = A078898(A003961(n)); A078898(m) = number of k
 *            <= m with the same least prime factor p as m = phi(m/p, primes <
 *            p), where phi(x,a) counts integers <= x with no prime factor
 *            among the first a primes (Legendre recursion)
 * Range:     u8 n <= 47, u16 n <= 3071, u32 n <= 3071, u64 n <= 3071,
 *            u128 n <= 3071, big any n
 * Validated: standard, pass, 112 terms through n = 3013 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A249810_phi(seq_t x, const uint64_t *pr, size_t a)
{
    if (x <= 0) return (seq_t)0;
    if (a == 0) return x;
    if ((seq_t)pr[a - 1] >= x) return (seq_t)1;
    seq_t res = x;
    for (size_t i = 0; i < a; ++i) {
        if ((seq_t)pr[i] > x) break;
        res -= A249810_phi(x / (seq_t)pr[i], pr, i);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* A003961(n) and smallest prime factor q of n */
    seq_t m = 1;
    uint64_t r = (uint64_t)n;
    uint64_t q = 0;
    for (uint64_t p = 2; p * p <= r; p++) {
        if (r % p == 0) {
            if (!q) q = p;
            uint64_t np = next_prime(p);
            while (r % p == 0) {
                r /= p;
                m *= (seq_t)np;
            }
        }
    }
    if (r > 1) {
        if (!q) q = r;
        m *= (seq_t)next_prime(r);
    }

    uint64_t P = next_prime(q);       /* smallest prime factor of m */
    seq_t x = m / (seq_t)P;
    if (x <= 1) return (seq_t)1;

    /* primes <= q */
    size_t cnt = 0, cap = 16;
    uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!pr) abort();
    for (uint64_t p = 2; p <= q; p = next_prime(p)) {
        if (cnt == cap) {
            cap *= 2;
            pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            if (!pr) abort();
        }
        pr[cnt++] = p;
    }
    seq_t res = A249810_phi(x, pr, cnt);
    free(pr);
    return res;
}
