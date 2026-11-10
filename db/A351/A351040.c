/*
 * A351040  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A336158(i) = A336158(j), A206787(i) = A206787(j) and A336651(i)
 *          = A336651(j) for all i, j >= 1.
 * https://oeis.org/A351040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = restricted growth sequence transform of [A336158(n),
 *            A206787(n), A336651(n)]; computed by scanning odd parts in order
 *            and numbering distinct triplets by first appearance
 * Range:     u8 n <= 592, u16 n <= 65539, u32 n <= 65539, u64 n <= 65539,
 *            u128 n <= 65539, big any n
 * Validated: standard, pass, 105 terms through n = 9175 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *ids_arr = NULL;      /* id for odd m = 2k+1 at index k */
static size_t ids_cnt = 0, ids_cap = 0;
static uint64_t *kA = NULL, *kB = NULL, *kC = NULL;
static size_t k_cnt = 0, k_cap = 0;

static void triplet_of_odd(uint64_t m, uint64_t *sig, uint64_t *s, uint64_t *r)
{
    static const uint64_t sp[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t ex[64];
    int cnt = 0;
    *s = 1; *r = 1;
    uint64_t x = m;
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            *s *= (p + 1);
            for (uint64_t i = 1; i < e; i++) *r *= p;
            ex[cnt++] = e;
        }
    }
    if (x > 1) { *s *= (x + 1); ex[cnt++] = 1; }
    for (int i = 0; i < cnt; i++)
        for (int j = i + 1; j < cnt; j++)
            if (ex[i] < ex[j]) { uint64_t t = ex[i]; ex[i] = ex[j]; ex[j] = t; }
    uint64_t g = 1;
    for (int i = 0; i < cnt && i < 16; i++)
        for (uint64_t e = 0; e < ex[i]; e++) g *= sp[i];
    *sig = g;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    while ((m & 1) == 0) m >>= 1;
    size_t target = (size_t)((m - 1) / 2);
    while (ids_cnt <= target) {
        uint64_t om = 2 * (uint64_t)ids_cnt + 1;
        uint64_t sg, s, r;
        triplet_of_odd(om, &sg, &s, &r);
        size_t found = k_cnt;
        for (size_t i = 0; i < k_cnt; i++)
            if (kA[i] == sg && kB[i] == s && kC[i] == r) { found = i; break; }
        if (found == k_cnt) {
            if (k_cnt == k_cap) {
                k_cap = k_cap ? k_cap * 2 : 1024;
                kA = (uint64_t *)realloc(kA, k_cap * sizeof(uint64_t));
                kB = (uint64_t *)realloc(kB, k_cap * sizeof(uint64_t));
                kC = (uint64_t *)realloc(kC, k_cap * sizeof(uint64_t));
            }
            kA[k_cnt] = sg; kB[k_cnt] = s; kC[k_cnt] = r;
            k_cnt++;
        }
        if (ids_cnt == ids_cap) {
            ids_cap = ids_cap ? ids_cap * 2 : 1024;
            ids_arr = (uint32_t *)realloc(ids_arr, ids_cap * sizeof(uint32_t));
        }
        ids_arr[ids_cnt++] = (uint32_t)found;
    }
    return (seq_t)(ids_arr[target] + 1);
}
