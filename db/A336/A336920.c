/*
 * A336920  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = [A329697(n), A331410(n), A336158(n)],
 *          for all i, j >= 1.
 * https://oeis.org/A336920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the triple
 *            [A329697(n), A331410(n), A336158(n)]; computed by scanning n =
 *            1, 2, ... and numbering distinct triples in order of first
 *            appearance (cached)
 * Range:     u8 n <= 2834, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8489 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A336920_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) { largest = 2; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

static uint64_t A336920_odd(uint64_t m)
{
    while (m && (m & 1ULL) == 0) m >>= 1;
    return m;
}

static uint64_t A336920_f1(uint64_t n)
{
    uint64_t m = A336920_odd(n), c = 0;
    while (m > 1) {
        uint64_t p = A336920_lpf(m);
        m -= m / p;
        m = A336920_odd(m);
        c++;
    }
    return c;
}

static uint64_t A336920_f2(uint64_t n)
{
    uint64_t m = A336920_odd(n), c = 0;
    while (m > 1) {
        uint64_t p = A336920_lpf(m);
        m += m / p;
        m = A336920_odd(m);
        c++;
    }
    return c;
}

static uint64_t A336920_f3(uint64_t n)
{
    uint64_t m = A336920_odd(n);
    uint64_t ex[64];
    size_t cnt = 0;
    if (m == 1) return 1;
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            uint64_t e = 0;
            while (m % f == 0) { m /= f; e++; }
            ex[cnt++] = e;
        }
    }
    if (m > 1) ex[cnt++] = 1;
    for (size_t i = 0; i < cnt; i++)
        for (size_t j = i + 1; j < cnt; j++)
            if (ex[i] < ex[j]) { uint64_t t = ex[i]; ex[i] = ex[j]; ex[j] = t; }
    static const uint64_t pr[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t r = 1;
    for (size_t i = 0; i < cnt && i < 20; i++)
        for (uint64_t e = 0; e < ex[i]; e++) r *= pr[i];
    return r;
}

static uint64_t *A336920_T = NULL;      /* distinct triples, 3 per id */
static size_t A336920_Dn = 0, A336920_Dcap = 0;
static uint32_t *A336920_R = NULL;      /* rgs values */
static size_t A336920_Rn = 0, A336920_Rcap = 0;
static int64_t *A336920_tab = NULL;
static size_t A336920_tabcap = 0;

static size_t A336920_hash(uint64_t x, uint64_t y, uint64_t z)
{
    uint64_t h = x * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29; h += y * 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 31; h += z * 0x94D049BB133111EBULL;
    h ^= h >> 32; h *= 0xD6E8FEB86659FD93ULL;
    h ^= h >> 32;
    return (size_t)h;
}

static void A336920_rehash(size_t newcap)
{
    free(A336920_tab);
    A336920_tab = (int64_t *)malloc(newcap * sizeof(int64_t));
    A336920_tabcap = newcap;
    for (size_t i = 0; i < newcap; i++) A336920_tab[i] = -1;
    for (size_t id = 0; id < A336920_Dn; id++) {
        size_t i = A336920_hash(A336920_T[3*id], A336920_T[3*id+1], A336920_T[3*id+2]) & (newcap - 1);
        while (A336920_tab[i] >= 0) i = (i + 1) & (newcap - 1);
        A336920_tab[i] = (int64_t)id;
    }
}

static uint32_t A336920_find_or_add(uint64_t x, uint64_t y, uint64_t z)
{
    if (A336920_tabcap == 0 || (A336920_Dn + 1) * 2 > A336920_tabcap)
        A336920_rehash(A336920_tabcap ? A336920_tabcap * 2 : 1024);
    size_t mask = A336920_tabcap - 1;
    size_t i = A336920_hash(x, y, z) & mask;
    while (A336920_tab[i] >= 0) {
        size_t id = (size_t)A336920_tab[i];
        if (A336920_T[3*id] == x && A336920_T[3*id+1] == y && A336920_T[3*id+2] == z)
            return (uint32_t)(id + 1);
        i = (i + 1) & mask;
    }
    if (A336920_Dn == A336920_Dcap) {
        A336920_Dcap = A336920_Dcap ? A336920_Dcap * 2 : 1024;
        A336920_T = (uint64_t *)realloc(A336920_T, A336920_Dcap * 3 * sizeof(uint64_t));
    }
    size_t id = A336920_Dn++;
    A336920_T[3*id] = x; A336920_T[3*id+1] = y; A336920_T[3*id+2] = z;
    A336920_tab[i] = (int64_t)id;
    return (uint32_t)(id + 1);
}

seq_t a(idx_t n)
{
    while (A336920_Rn < (size_t)n) {
        uint64_t k = (uint64_t)A336920_Rn + 1;
        uint32_t v = A336920_find_or_add(A336920_f1(k), A336920_f2(k), A336920_f3(k));
        if (A336920_Rn == A336920_Rcap) {
            A336920_Rcap = A336920_Rcap ? A336920_Rcap * 2 : 1024;
            A336920_R = (uint32_t *)realloc(A336920_R, A336920_Rcap * sizeof(uint32_t));
        }
        A336920_R[A336920_Rn++] = v;
    }
    return (seq_t)A336920_R[(size_t)n - 1];
}
