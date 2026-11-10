/*
 * A368905  a(n) = 1 if A342001(A005940(1+n)) is not divisible by p^p for any
 *          prime p, otherwise 0.
 * https://oeis.org/A368905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = 1 if x = A342001(A005940(1+n)) is
 *            not divisible by p^p for any prime p, otherwise 0. A005940(1+n)
 *            = Prod prime(k)^e_k is read from the binary digits of n. Then x
 *            = Sum_k e_k * rad/prime(k), where rad is the product of the
 *            primes that appear. x mod p^p is computed for e...
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 138 terms through n = 9374 (2026-11-10)
 * Helpers:   mulmod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mulmod_u128.h"

static uint64_t A368905_nth_prime(int k)
{
    /* k-th prime (k >= 1) by trial division */
    uint64_t c = 1;
    int found = 0;
    while (found < k) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) found++;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    int exps[72];
    uint64_t pr[72];
    for (int i = 0; i < 72; ++i) { exps[i] = 0; pr[i] = 0; }

    /* exponents of A005940(1+n): a 1-bit followed by k-1 zeros adds to prime(k) */
    idx_t m = n;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            if (i < 72) exps[i]++;
            m >>= 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    int top = i < 71 ? i : 71;

    int idx[72];
    int cnt = 0;
    for (int k = 1; k <= top; ++k) {
        if (exps[k] > 0) {
            idx[cnt++] = k;
            pr[k] = A368905_nth_prime(k);
        }
    }

    static const unsigned qs[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };
    for (size_t t = 0; t < sizeof(qs) / sizeof(qs[0]); ++t) {
        unsigned q = qs[t];
        unsigned __int128 M = 1;
        for (unsigned j = 0; j < q; ++j) M *= (unsigned __int128)q;

        unsigned __int128 xm = 0;
        for (int u = 0; u < cnt; ++u) {
            unsigned __int128 term = (unsigned __int128)(unsigned)exps[idx[u]] % M;
            for (int v = 0; v < cnt; ++v) {
                if (v == u) continue;
                term = mulmod_u128(term, (unsigned __int128)pr[idx[v]] % M, M);
            }
            xm += term;
            if (xm >= M) xm -= M;
        }
        if (xm == 0) return (seq_t)0;
    }
    return (seq_t)1;
}
