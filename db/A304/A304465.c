/*
 * A304465  If n is prime, set a(n) = 1. Otherwise, start with the multiset of
 *          prime factors of n, and given a multiset take the multiset of its
 *          multiplicities. Repeating this until a multiset of size 1 is
 *          reached, set a(n) to the unique element of this multiset.
 * https://oeis.org/A304465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime, a(p^k) = k for prime powers, a(product
 *            of m distinct primes) = m. More generally, start with the
 *            multiset of prime factors of n, then repeatedly replace a
 *            multiset by the multiset of its multiplicities until a single
 *            element remains; that element is a(n)
 * Range:     u8 n <= 9917, u16 n <= 9917, u32 n <= 9917, u64 n <= 9917,
 *            u128 n <= 9917, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    /* factorize n */
    /* maximum distinct prime factors for n up to 64-bit is small (≤24) */
    uint64_t primes[32], exps[32];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* current multiset: exponents of prime factorization */
    seq_t cur[32];
    size_t cur_len = cnt;
    for (size_t i = 0; i < cnt; ++i) cur[i] = (seq_t)exps[i];

    /* iterate until a single element remains */
    while (cur_len > 1) {
        seq_t next[32];
        size_t next_len = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            seq_t val = cur[i];
            /* skip if we've already processed this value */
            int seen = 0;
            for (size_t k = 0; k < i; ++k)
                if (cur[k] == val) { seen = 1; break; }
            if (seen) continue;
            seq_t count = 0;
            for (size_t j = 0; j < cur_len; ++j)
                if (cur[j] == val) ++count;
            next[next_len++] = count;
        }
        for (size_t i = 0; i < next_len; ++i) cur[i] = next[i];
        cur_len = next_len;
    }
    return cur[0];
}
