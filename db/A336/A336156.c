/*
 * A336156  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A007814(1+i) = A007814(1+j) and A336158(i) = A336158(j), for
 *          all i, j >= 1.
 * https://oeis.org/A336156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A007814(1+n), A336158(n)]; computed by scanning 1..n and
 *            numbering distinct pairs in order of first appearance
 * Range:     u8 n <= 50174, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9870 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t rgs_sig(uint64_t m)
{
    static const uint64_t sp[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t ex[20];
    int cnt = 0;
    while ((m & 1) == 0) m >>= 1;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            ex[cnt++] = e;
        }
    }
    if (m > 1) ex[cnt++] = 1;
    for (int i = 0; i < cnt; i++)
        for (int j = i + 1; j < cnt; j++)
            if (ex[i] < ex[j]) { uint64_t t = ex[i]; ex[i] = ex[j]; ex[j] = t; }
    uint64_t r = 1;
    for (int i = 0; i < cnt; i++)
        for (uint64_t e = 0; e < ex[i]; e++) r *= sp[i];
    return r;
}

static uint64_t *rgs_k1 = NULL, *rgs_k2 = NULL, *rgs_val = NULL;
static uint64_t rgs_done = 0, rgs_cap = 0, rgs_distinct = 0;

seq_t a(idx_t n)
{
    if (n > rgs_done) {
        if ((uint64_t)n > rgs_cap) {
            uint64_t nc = rgs_cap ? rgs_cap : 1024;
            while (nc < (uint64_t)n) nc *= 2;
            rgs_k1 = (uint64_t *)realloc(rgs_k1, nc * sizeof(uint64_t));
            rgs_k2 = (uint64_t *)realloc(rgs_k2, nc * sizeof(uint64_t));
            rgs_val = (uint64_t *)realloc(rgs_val, (nc + 1) * sizeof(uint64_t));
            rgs_cap = nc;
        }
        for (uint64_t i = rgs_done + 1; i <= (uint64_t)n; i++) {
            uint64_t t = i + 1, k1 = 0;
            while ((t & 1) == 0) { t >>= 1; k1++; }
            uint64_t k2 = rgs_sig(i);
            uint64_t found = 0;
            for (uint64_t j = 0; j < rgs_distinct; j++) {
                if (rgs_k1[j] == k1 && rgs_k2[j] == k2) { found = j + 1; break; }
            }
            if (!found) {
                rgs_k1[rgs_distinct] = k1;
                rgs_k2[rgs_distinct] = k2;
                rgs_distinct++;
                found = rgs_distinct;
            }
            rgs_val[i] = found;
        }
        rgs_done = (uint64_t)n;
    }
    return (seq_t)rgs_val[n];
}
