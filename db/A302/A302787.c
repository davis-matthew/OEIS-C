/*
 * A302787  a(1) = 0; for n > 1, a(n) = A000265(A052331(n)).
 * https://oeis.org/A302787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = A000265(A052331(n)), i.e. the odd
 *            part of the binary encoding of the Fermi-Dirac factorization of
 *            n: bit k-1 is set when the k-th number of the form p^(2^j)
 *            (2,3,4,5,7,9,11,13,16,...) divides n in its Fermi-Dirac
 *            factorization; trailing zero bits are then removed
 * Range:     u8 n <= 31, u16 n <= 81, u32 n <= 205, u64 n <= 525,
 *            u128 n <= 1281, big any n
 * Validated: standard, pass, 107 terms through n = 8121 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    size_t N = (size_t)n;
    unsigned char *comp = (unsigned char *)calloc(N + 1, 1);
    unsigned char *flag = (unsigned char *)calloc(N + 1, 1);
    size_t *rank = (size_t *)calloc(N + 1, sizeof(size_t));

    for (size_t i = 2; i <= N; ++i) {
        if (!comp[i]) {
            if (i <= N / i)
                for (size_t j = i * i; j <= N; j += i)
                    comp[j] = 1;
            uint64_t v = i;
            for (;;) {
                flag[v] = 1;
                if (v > N / v)
                    break;
                v = v * v;
            }
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (flag[i])
            ++cnt;
        rank[i] = cnt;
    }

    unsigned char *bits = (unsigned char *)calloc(cnt + 2, 1);
    size_t minidx = 0, maxidx = 0;
    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; t > 1; ++p) {
        if (p * p > t)
            p = t;
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            uint64_t v = p;
            while (e) {
                if (e & 1U) {
                    size_t idx = rank[v];
                    bits[idx] = 1;
                    if (minidx == 0 || idx < minidx)
                        minidx = idx;
                    if (idx > maxidx)
                        maxidx = idx;
                }
                e >>= 1;
                if (e)
                    v = v * v;
            }
        }
    }

    seq_t result = 0;
    if (minidx >= 1) {
        size_t i = maxidx;
        for (;;) {
            result = result * 2 + (seq_t)bits[i];
            if (i == minidx)
                break;
            --i;
        }
    }

    free(comp);
    free(flag);
    free(rank);
    free(bits);
    return result;
}
