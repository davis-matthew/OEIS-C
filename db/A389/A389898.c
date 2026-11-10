/*
 * A389898  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A126760(i) = A126760(j), A343029(i) = A343029(j) and A343030(i)
 *          = A343030(j) for all i, j >= 1.
 * https://oeis.org/A389898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of the triple (A126760(n),
 *            A343029(n), A343030(n)) among triples of indices 1..n (labels
 *            assigned in order of first appearance); computed by scan with
 *            hash table
 * Range:     u8 n <= 366, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 108 terms through n = 9826 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A389898_f126760(uint64_t n)
{
    uint64_t result = 0, m = n;
    while (m > 0) {
        uint64_t r = m % 6;
        if (r == 1) { result = 2 * ((m - 1) / 6) + 1; break; }
        if (r == 5) { result = 2 * ((m - 5) / 6) + 2; break; }
        if ((m & 1) == 0) { m >>= 1; continue; }
        if (m % 3 == 0) { m /= 3; continue; }
        break;
    }
    return result;
}

static uint64_t A389898_f343029(uint64_t n)
{
    uint64_t res = 0, ones = 0, pos = 0, cur = n;
    while (cur) {
        if (cur & 1ULL) {
            if ((pos & 1ULL) == (ones & 1ULL)) res++;
            ones++;
        }
        cur >>= 1;
        pos++;
    }
    return res;
}

static uint64_t A389898_f343030(uint64_t n)
{
    uint64_t res = 0, zeros = 0, t = n;
    while (t) {
        if (t & 1ULL) {
            if (zeros & 1) res++;
        } else zeros++;
        t >>= 1;
    }
    return res;
}

static uint64_t *A389898_lab = 0;   /* label per index */
static uint64_t A389898_labcap = 0;
static uint64_t A389898_done = 0;
static uint64_t *A389898_kx = 0, *A389898_k1 = 0, *A389898_k2 = 0;
static uint64_t A389898_cnt = 0, A389898_kcap = 0;
static uint64_t *A389898_tab = 0;
static uint64_t A389898_tcap = 0;

static uint64_t A389898_hash(uint64_t x, uint64_t c1, uint64_t c2)
{
    uint64_t h = x * 0x9E3779B97F4A7C15ULL;
    h ^= (c1 * 131 + c2 + 1) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return h;
}

static void A389898_rehash(uint64_t newcap)
{
    uint64_t i;
    free(A389898_tab);
    A389898_tab = (uint64_t *)calloc(newcap, sizeof(uint64_t));
    A389898_tcap = newcap;
    for (i = 0; i < A389898_cnt; i++) {
        uint64_t h = A389898_hash(A389898_kx[i], A389898_k1[i], A389898_k2[i]) & (newcap - 1);
        while (A389898_tab[h]) h = (h + 1) & (newcap - 1);
        A389898_tab[h] = i + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A389898_done) {
        uint64_t m;
        if (n + 1 > A389898_labcap) {
            uint64_t nc = A389898_labcap ? A389898_labcap : 1024;
            while (nc < n + 1) nc *= 2;
            A389898_lab = (uint64_t *)realloc(A389898_lab, nc * sizeof(uint64_t));
            A389898_labcap = nc;
        }
        if (!A389898_tab) A389898_rehash(1024);
        for (m = A389898_done + 1; m <= n; m++) {
            uint64_t x = A389898_f126760(m);
            uint64_t c1 = A389898_f343029(m);
            uint64_t c2 = A389898_f343030(m);
            uint64_t h = A389898_hash(x, c1, c2) & (A389898_tcap - 1);
            uint64_t found = 0;
            while (A389898_tab[h]) {
                uint64_t k = A389898_tab[h] - 1;
                if (A389898_kx[k] == x && A389898_k1[k] == c1 && A389898_k2[k] == c2) {
                    found = k + 1;
                    break;
                }
                h = (h + 1) & (A389898_tcap - 1);
            }
            if (!found) {
                if (A389898_cnt + 1 > A389898_kcap) {
                    uint64_t nk = A389898_kcap ? A389898_kcap * 2 : 1024;
                    A389898_kx = (uint64_t *)realloc(A389898_kx, nk * sizeof(uint64_t));
                    A389898_k1 = (uint64_t *)realloc(A389898_k1, nk * sizeof(uint64_t));
                    A389898_k2 = (uint64_t *)realloc(A389898_k2, nk * sizeof(uint64_t));
                    A389898_kcap = nk;
                }
                A389898_kx[A389898_cnt] = x;
                A389898_k1[A389898_cnt] = c1;
                A389898_k2[A389898_cnt] = c2;
                A389898_cnt++;
                found = A389898_cnt;
                A389898_tab[h] = found;
                if (A389898_cnt * 2 > A389898_tcap) A389898_rehash(A389898_tcap * 2);
            }
            A389898_lab[m] = found;
        }
        A389898_done = n;
    }
    return (seq_t)A389898_lab[n];
}
