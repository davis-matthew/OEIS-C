/*
 * A369259  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003557(i) = A003557(j), A048250(i) = A048250(j) and A342671(i)
 *          = A342671(j), for all i, j >= 1.
 * https://oeis.org/A369259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the triple *
 *            [A003557(n), A048250(n), A342671(n)], where A003557(n) = Product
 *            p^(e-1), * A048250(n) = Product_{p|n} (p+1), A342671(n) =
 *            gcd(sigma(n), A003961(n)). * Computed by scanning k = 1..n and
 *            giving each new triple the next label
 * Range:     u8 n <= 300, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9637 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A369259_isprime(uint64_t q) {
    if (q < 2) return false;
    for (uint64_t d = 2; d * d <= q; d++)
        if (q % d == 0) return false;
    return true;
}

static uint64_t A369259_nextprime(uint64_t p) {
    uint64_t q = p + 1;
    while (!A369259_isprime(q)) q++;
    return q;
}

static uint64_t A369259_gcd(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t r = x % y;
        x = y;
        y = r;
    }
    return x;
}

static void A369259_triple(uint64_t m, uint64_t *t1, uint64_t *t2, uint64_t *t3) {
    uint64_t r1 = 1, r2 = 1, sg = 1, a61 = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            uint64_t term = 1, ssum = 1;
            for (unsigned k = 0; k < e; k++) {
                term *= p;
                ssum += term;
                if (k + 1 < e) r1 *= p;
            }
            sg *= ssum;
            r2 *= (p + 1);
            uint64_t q = A369259_nextprime(p);
            for (unsigned k = 0; k < e; k++) a61 *= q;
        }
    }
    if (m > 1) {
        sg *= (m + 1);
        r2 *= (m + 1);
        a61 *= A369259_nextprime(m);
    }
    *t1 = r1;
    *t2 = r2;
    *t3 = A369259_gcd(sg, a61);
}

static uint32_t *A369259_ids = NULL;
static size_t A369259_computed = 0, A369259_idcap = 0;

static uint64_t *A369259_cls = NULL;   /* 3 entries per class */
static size_t A369259_nd = 0, A369259_dcap = 0;

static int64_t *A369259_ht = NULL;     /* class index or -1 */
static size_t A369259_hcap = 0;

static uint64_t A369259_hash(uint64_t a1, uint64_t a2, uint64_t a3) {
    uint64_t h = a1 * 0x9E3779B97F4A7C15ULL;
    h ^= a2 + 0x632BE59BD9B4E019ULL + (h << 6) + (h >> 2);
    h ^= a3 * 0xC2B2AE3D27D4EB4FULL + (h << 6) + (h >> 2);
    h ^= h >> 31;
    return h;
}

static void A369259_rehash(size_t ncap) {
    int64_t *nt = (int64_t *)malloc(ncap * sizeof(int64_t));
    for (size_t i = 0; i < ncap; i++) nt[i] = -1;
    for (size_t c = 0; c < A369259_nd; c++) {
        uint64_t *t = A369259_cls + 3 * c;
        size_t pos = (size_t)(A369259_hash(t[0], t[1], t[2]) & (ncap - 1));
        while (nt[pos] != -1) pos = (pos + 1) & (ncap - 1);
        nt[pos] = (int64_t)c;
    }
    free(A369259_ht);
    A369259_ht = nt;
    A369259_hcap = ncap;
}

seq_t a(idx_t n) {
    if (A369259_hcap == 0) A369259_rehash(1024);
    while (A369259_computed < (size_t)n) {
        uint64_t idx = (uint64_t)A369259_computed + 1;
        uint64_t t1, t2, t3;
        A369259_triple(idx, &t1, &t2, &t3);

        size_t pos = (size_t)(A369259_hash(t1, t2, t3) & (A369259_hcap - 1));
        int64_t found = -1;
        while (A369259_ht[pos] != -1) {
            uint64_t *t = A369259_cls + 3 * (size_t)A369259_ht[pos];
            if (t[0] == t1 && t[1] == t2 && t[2] == t3) {
                found = A369259_ht[pos];
                break;
            }
            pos = (pos + 1) & (A369259_hcap - 1);
        }
        if (found < 0) {
            if (A369259_nd == A369259_dcap) {
                size_t nc = A369259_dcap ? A369259_dcap * 2 : 1024;
                A369259_cls = (uint64_t *)realloc(A369259_cls, nc * 3 * sizeof(uint64_t));
                A369259_dcap = nc;
            }
            A369259_cls[3 * A369259_nd] = t1;
            A369259_cls[3 * A369259_nd + 1] = t2;
            A369259_cls[3 * A369259_nd + 2] = t3;
            found = (int64_t)A369259_nd;
            A369259_ht[pos] = found;
            A369259_nd++;
            if (2 * A369259_nd > A369259_hcap) A369259_rehash(A369259_hcap * 2);
        }

        if (A369259_computed >= A369259_idcap) {
            size_t nc = A369259_idcap ? A369259_idcap * 2 : 1024;
            A369259_ids = (uint32_t *)realloc(A369259_ids, nc * sizeof(uint32_t));
            A369259_idcap = nc;
        }
        A369259_ids[A369259_computed] = (uint32_t)(found + 1);
        A369259_computed++;
    }
    return (seq_t)A369259_ids[(size_t)n - 1];
}
