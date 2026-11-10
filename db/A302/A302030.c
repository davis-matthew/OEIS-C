/*
 * A302030  a(n) = 1+A006068(A052331(n)).
 * https://oeis.org/A302030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A006068(A052331(n)), where A052331(n) = sum of
 *            2^(r-1) over the Fermi-Dirac factors p^(2^k) of n, r = rank of
 *            p^(2^k) among all prime powers of the form p^(2^k) (A050376),
 *            and A006068 is the inverse Gray code
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 100, u64 n <= 256,
 *            u128 n <= 630, big any n
 * Validated: standard, pass, 82 terms through n = 4083 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t A302030_gray_inv(seq_t x)
{
    seq_t result = x;
    seq_t shifted = x;
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    return result;
}

static seq_t A302030_pow2(uint32_t k)
{
    seq_t pw = 1;
    for (uint32_t t = 0; t < k; ++t) pw *= 2;
    return pw;
}

static seq_t A302030_fd(idx_t n)
{
    size_t N = (size_t)n;
    seq_t result = 0;
    if (N < 2) return result;

    uint32_t *rank = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc(N + 1, 1);
    if (!rank || !comp) { free(rank); free(comp); return result; }

    for (size_t i = 2; i <= N; ++i) {
        if (!comp[i]) {
            if (i <= N / i)
                for (size_t j = i * i; j <= N; j += i) comp[j] = 1;
            /* i is prime: mark i^(2^k) <= N */
            size_t v = i;
            while (1) {
                rank[v] = 1;
                if (v > N / v) break;
                v = v * v;
            }
        }
    }
    uint32_t cnt = 0;
    for (size_t i = 1; i <= N; ++i) {
        if (rank[i]) { ++cnt; rank[i] = cnt; }
    }

    size_t temp = N;
    for (size_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            size_t v = p;
            while (e) {
                if (e & 1U) result += A302030_pow2(rank[v] - 1);
                e >>= 1;
                if (e) v = v * v;
            }
        }
    }
    if (temp > 1) result += A302030_pow2(rank[temp] - 1);

    free(rank);
    free(comp);
    return result;
}

seq_t a(idx_t n)
{
    return A302030_gray_inv(A302030_fd(n)) + 1;
}
