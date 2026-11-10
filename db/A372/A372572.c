/*
 * A372572  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A009194(i) = A009194(j), A322361(i) = A322361(j) and A342671(i)
 *          = A342671(j), for all i, j >= 1.
 * https://oeis.org/A372572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the triple
 *            [A009194(n), A322361(n), A342671(n)]: a(n) = 1 + number of
 *            distinct triples seen first before the first occurrence of the
 *            triple at n; computed by scanning k = 1..n with a hash table
 * Range:     u8 n <= 1631, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9837 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef unsigned __int128 rgs_u128;

static rgs_u128 rgs_gcd(rgs_u128 a, rgs_u128 b)
{
    while (b != 0) {
        rgs_u128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static uint64_t rgs_next_prime(uint64_t p)
{
    uint64_t q = p + 1;
    for (;; ++q) {
        bool ok = (q >= 2);
        for (uint64_t d = 2; d * d <= q; ++d) {
            if (q % d == 0) { ok = false; break; }
        }
        if (ok) return q;
    }
}

static void rgs_triple(uint64_t n, uint64_t out[3])
{
    rgs_u128 sigma = 1, a3 = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            rgs_u128 pw = 1, sum = 1;
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
                pw *= p;
                sum += pw;
            }
            sigma *= sum;
            uint64_t q = rgs_next_prime(p);
            for (uint64_t k = 0; k < e; ++k) a3 *= q;
        }
    }
    if (m > 1) {
        sigma *= ((rgs_u128)m + 1);
        a3 *= rgs_next_prime(m);
    }
    out[0] = (uint64_t)rgs_gcd((rgs_u128)n, sigma);
    out[1] = (uint64_t)rgs_gcd((rgs_u128)n, a3);
    out[2] = (uint64_t)rgs_gcd(sigma, a3);
}

static uint64_t rgs_hash(const uint64_t t[3])
{
    uint64_t h = t[0] * 0x9E3779B97F4A7C15ULL;
    h ^= (h >> 29);
    h += t[1] * 0xC2B2AE3D27D4EB4FULL;
    h ^= (h >> 31);
    h += t[2] * 0x165667B19E3779F9ULL;
    h ^= (h >> 32);
    h *= 0xD6E8FEB86659FD93ULL;
    h ^= (h >> 28);
    return h;
}

static uint32_t *rgs_ids = NULL;      /* id of each k (1-based index k-1) */
static size_t rgs_done = 0, rgs_idcap = 0;
static uint64_t *rgs_tr = NULL;       /* distinct triples, 3 each */
static size_t rgs_nd = 0, rgs_trcap = 0;
static uint32_t *rgs_tab = NULL;      /* stores distinct index + 1, 0 empty */
static size_t rgs_tabsz = 0;

static void rgs_rebuild(size_t newsz)
{
    free(rgs_tab);
    rgs_tab = (uint32_t *)calloc(newsz, sizeof(uint32_t));
    rgs_tabsz = newsz;
    for (size_t i = 0; i < rgs_nd; ++i) {
        size_t h = (size_t)(rgs_hash(&rgs_tr[3 * i]) & (newsz - 1));
        while (rgs_tab[h]) h = (h + 1) & (newsz - 1);
        rgs_tab[h] = (uint32_t)(i + 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    if (N > rgs_idcap) {
        size_t nc = rgs_idcap ? rgs_idcap : 1024;
        while (nc < N) nc *= 2;
        rgs_ids = (uint32_t *)realloc(rgs_ids, nc * sizeof(uint32_t));
        rgs_idcap = nc;
    }
    if (!rgs_tab) rgs_rebuild(4096);
    while (rgs_done < N) {
        uint64_t t[3];
        rgs_triple((uint64_t)(rgs_done + 1), t);
        size_t h = (size_t)(rgs_hash(t) & (rgs_tabsz - 1));
        uint32_t found = 0;
        while (rgs_tab[h]) {
            const uint64_t *u = &rgs_tr[3 * (size_t)(rgs_tab[h] - 1)];
            if (u[0] == t[0] && u[1] == t[1] && u[2] == t[2]) {
                found = rgs_tab[h];
                break;
            }
            h = (h + 1) & (rgs_tabsz - 1);
        }
        if (!found) {
            if (rgs_nd + 1 > rgs_trcap) {
                size_t nc = rgs_trcap ? rgs_trcap * 2 : 1024;
                rgs_tr = (uint64_t *)realloc(rgs_tr, nc * 3 * sizeof(uint64_t));
                rgs_trcap = nc;
            }
            rgs_tr[3 * rgs_nd] = t[0];
            rgs_tr[3 * rgs_nd + 1] = t[1];
            rgs_tr[3 * rgs_nd + 2] = t[2];
            ++rgs_nd;
            found = (uint32_t)rgs_nd;
            rgs_tab[h] = found;
            if (rgs_nd * 2 > rgs_tabsz) rgs_rebuild(rgs_tabsz * 2);
        }
        rgs_ids[rgs_done] = found;
        ++rgs_done;
    }
    return (seq_t)rgs_ids[N - 1];
}
