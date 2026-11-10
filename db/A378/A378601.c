/*
 * A378601  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A046523(i) = A046523(j) and A083206(i) = A083206(j), for all i,
 *          j >= 1.
 * https://oeis.org/A378601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A046523(n), A083206(n)]; computed by scanning k=1..n and
 *            numbering each new pair in order of first appearance
 * Range:     u8 n <= 2001, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9144 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t A378601_sig(uint64_t n)
{
    uint64_t ex[20];
    int c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            ex[c++] = (uint64_t)e;
        }
    }
    if (n > 1) ex[c++] = 1;
    /* sort descending */
    for (int i = 1; i < c; ++i) {
        uint64_t v = ex[i];
        int j = i - 1;
        while (j >= 0 && ex[j] < v) { ex[j + 1] = ex[j]; --j; }
        ex[j + 1] = v;
    }
    static const uint64_t pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t r = 1;
    for (int i = 0; i < c; ++i)
        for (uint64_t e = 0; e < ex[i]; ++e) r *= pr[i];
    return r;
}

static uint64_t A378601_part(uint64_t n)
{
    uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t) * 4096);
    size_t dcnt = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i != n / i) divs[dcnt++] = n / i;
        }
    }
    uint64_t sg = 0;
    for (size_t i = 0; i < dcnt; ++i) sg += divs[i];
    if (sg & 1) { free(divs); return 0; }
    size_t target = (size_t)(sg >> 1);
    uint64_t *dp = (uint64_t *)calloc(target + 1, sizeof(uint64_t));
    dp[0] = 1;
    for (size_t k = 0; k < dcnt; ++k) {
        size_t d = (size_t)divs[k];
        if (d > target) continue;
        for (size_t s = target; s >= d; --s)
            dp[s] += dp[s - d];
    }
    uint64_t r = dp[target] / 2;
    free(dp);
    free(divs);
    return r;
}

seq_t a(idx_t n)
{
    static uint32_t *ids = NULL;
    static size_t cap = 0, done = 0;
    static uint64_t *ks = NULL, *vs = NULL;
    static size_t pcnt = 0, pcap = 0;

    size_t N = (size_t)n;
    if (N > cap) {
        size_t nc = cap ? cap : 1024;
        while (nc < N) nc *= 2;
        ids = (uint32_t *)realloc(ids, (nc + 1) * sizeof(uint32_t));
        cap = nc;
    }
    while (done < N) {
        uint64_t k = (uint64_t)done + 1;
        uint64_t s = A378601_sig(k);
        uint64_t v = A378601_part(k);
        size_t f = pcnt;
        for (size_t i = 0; i < pcnt; ++i) {
            if (ks[i] == s && vs[i] == v) { f = i; break; }
        }
        if (f == pcnt) {
            if (pcnt == pcap) {
                pcap = pcap ? pcap * 2 : 256;
                ks = (uint64_t *)realloc(ks, pcap * sizeof(uint64_t));
                vs = (uint64_t *)realloc(vs, pcap * sizeof(uint64_t));
            }
            ks[pcnt] = s;
            vs[pcnt] = v;
            ++pcnt;
        }
        ids[k] = (uint32_t)(f + 1);
        ++done;
    }
    return (seq_t)ids[N];
}
