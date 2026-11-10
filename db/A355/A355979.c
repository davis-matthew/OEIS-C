/*
 * A355979  Indices of primes in A001835.
 * https://oeis.org/A355979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th index k with A001835(k) prime, where A001835(k) =
 *            4*A001835(k-1) - A001835(k-2), A001835(0)=A001835(1)=1; computed
 *            by search with a multiword Miller-Rabin test
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A355979_P2 = 0, *A355979_P1 = 0;
static size_t A355979_cap = 0, A355979_len = 0;
static uint64_t A355979_K = 0;
static uint64_t *A355979_found = 0;
static size_t A355979_nf = 0, A355979_nfcap = 0;

static void A355979_grow(void) {
    size_t nc = A355979_cap ? A355979_cap * 2 : 8;
    uint32_t *a = (uint32_t *)calloc(nc, sizeof(uint32_t));
    uint32_t *b = (uint32_t *)calloc(nc, sizeof(uint32_t));
    if (A355979_cap) {
        memcpy(a, A355979_P1, A355979_cap * sizeof(uint32_t));
        memcpy(b, A355979_P2, A355979_cap * sizeof(uint32_t));
        free(A355979_P1);
        free(A355979_P2);
    }
    A355979_P1 = a;
    A355979_P2 = b;
    A355979_cap = nc;
}

static void A355979_step(void) {
    if (A355979_len + 2 > A355979_cap) A355979_grow();
    uint64_t carry = 0;
    int64_t borrow = 0;
    size_t lim = A355979_len + 1;
    for (size_t i = 0; i < lim; i++) {
        uint64_t prod = (uint64_t)A355979_P1[i] * 4 + carry;
        carry = prod >> 32;
        int64_t d = (int64_t)(uint32_t)prod - (int64_t)A355979_P2[i] - borrow;
        borrow = d < 0 ? 1 : 0;
        A355979_P2[i] = (uint32_t)d;
    }
    uint32_t *t = A355979_P1;
    A355979_P1 = A355979_P2;
    A355979_P2 = t;
    size_t l = lim;
    while (l > 1 && A355979_P1[l - 1] == 0) l--;
    A355979_len = l;
    A355979_K++;
}

static void A355979_montmul(uint32_t *res, const uint32_t *a, const uint32_t *b,
                            const uint32_t *n, size_t L, uint32_t ninv, uint32_t *t) {
    memset(t, 0, (L + 2) * sizeof(uint32_t));
    for (size_t i = 0; i < L; i++) {
        uint64_t carry = 0, s;
        for (size_t j = 0; j < L; j++) {
            s = (uint64_t)t[j] + (uint64_t)a[j] * b[i] + carry;
            t[j] = (uint32_t)s;
            carry = s >> 32;
        }
        s = (uint64_t)t[L] + carry;
        t[L] = (uint32_t)s;
        t[L + 1] = (uint32_t)(s >> 32);
        uint32_t m = t[0] * ninv;
        s = (uint64_t)t[0] + (uint64_t)m * n[0];
        carry = s >> 32;
        for (size_t j = 1; j < L; j++) {
            s = (uint64_t)t[j] + (uint64_t)m * n[j] + carry;
            t[j - 1] = (uint32_t)s;
            carry = s >> 32;
        }
        s = (uint64_t)t[L] + carry;
        t[L - 1] = (uint32_t)s;
        s = (uint64_t)t[L + 1] + (s >> 32);
        t[L] = (uint32_t)s;
        t[L + 1] = 0;
    }
    bool ge = t[L] != 0;
    if (!ge) {
        ge = true;
        for (size_t j = L; j-- > 0;) {
            if (t[j] != n[j]) { ge = t[j] > n[j]; break; }
        }
    }
    if (ge) {
        int64_t br = 0;
        for (size_t j = 0; j < L; j++) {
            int64_t d = (int64_t)t[j] - (int64_t)n[j] - br;
            br = d < 0 ? 1 : 0;
            t[j] = (uint32_t)d;
        }
    }
    memcpy(res, t, L * sizeof(uint32_t));
}

static void A355979_dbl_mod(uint32_t *x, const uint32_t *n, size_t L) {
    uint32_t c = 0;
    for (size_t j = 0; j < L; j++) {
        uint32_t nc = x[j] >> 31;
        x[j] = (x[j] << 1) | c;
        c = nc;
    }
    bool ge = c != 0;
    if (!ge) {
        ge = true;
        for (size_t j = L; j-- > 0;) {
            if (x[j] != n[j]) { ge = x[j] > n[j]; break; }
        }
    }
    if (ge) {
        int64_t br = 0;
        for (size_t j = 0; j < L; j++) {
            int64_t d = (int64_t)x[j] - (int64_t)n[j] - br;
            br = d < 0 ? 1 : 0;
            x[j] = (uint32_t)d;
        }
    }
}

static bool A355979_is_prp(const uint32_t *n, size_t L) {
    static const uint32_t sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    if (L == 1 && n[0] < 2) return false;
    for (size_t i = 0; i < sizeof(sp) / sizeof(sp[0]); i++) {
        uint64_t r = 0;
        for (size_t j = L; j-- > 0;) r = ((r << 32) | n[j]) % sp[i];
        if (r == 0) return (L == 1 && n[0] == sp[i]);
    }
    if (L == 1 && n[0] < 97u * 97u) return true;

    uint32_t *buf = (uint32_t *)calloc(L * 6 + 2 + (L + 2), sizeof(uint32_t));
    uint32_t *one = buf, *rr = buf + L, *bm = buf + 2 * L, *r = buf + 3 * L,
             *mone = buf + 4 * L, *nm1 = buf + 5 * L, *t = buf + 6 * L;
    uint32_t inv = n[0];
    for (int i = 0; i < 5; i++) inv *= 2u - n[0] * inv;
    uint32_t ninv = (uint32_t)(0u - inv);

    one[0] = 1;
    for (size_t i = 0; i < 32 * L; i++) A355979_dbl_mod(one, n, L);
    memcpy(rr, one, L * sizeof(uint32_t));
    for (size_t i = 0; i < 32 * L; i++) A355979_dbl_mod(rr, n, L);
    {
        int64_t br = 0;
        for (size_t j = 0; j < L; j++) {
            int64_t d = (int64_t)n[j] - (int64_t)one[j] - br;
            br = d < 0 ? 1 : 0;
            mone[j] = (uint32_t)d;
        }
    }
    memcpy(nm1, n, L * sizeof(uint32_t));
    nm1[0] &= ~1u;
    size_t s = 0;
    while (!((nm1[s / 32] >> (s % 32)) & 1u)) s++;
    size_t tb = 32 * L;
    while (tb > 0 && !((nm1[(tb - 1) / 32] >> ((tb - 1) % 32)) & 1u)) tb--;

    static const uint32_t bases[] = {2, 3, 5, 7, 11, 13};
    bool prime = true;
    for (size_t bi = 0; bi < 6 && prime; bi++) {
        uint32_t *plain = t + L + 2; (void)plain;
        uint32_t *bb = bm;
        memset(bb, 0, L * sizeof(uint32_t));
        bb[0] = bases[bi];
        A355979_montmul(bb, bb, rr, n, L, ninv, t);
        memcpy(r, one, L * sizeof(uint32_t));
        for (size_t bit = tb; bit-- > s;) {
            A355979_montmul(r, r, r, n, L, ninv, t);
            if ((nm1[bit / 32] >> (bit % 32)) & 1u)
                A355979_montmul(r, r, bb, n, L, ninv, t);
        }
        if (memcmp(r, one, L * sizeof(uint32_t)) == 0 ||
            memcmp(r, mone, L * sizeof(uint32_t)) == 0) continue;
        bool ok = false;
        for (size_t i = 1; i < s; i++) {
            A355979_montmul(r, r, r, n, L, ninv, t);
            if (memcmp(r, mone, L * sizeof(uint32_t)) == 0) { ok = true; break; }
        }
        if (!ok) prime = false;
    }
    free(buf);
    return prime;
}

seq_t a(idx_t n) {
    if (A355979_cap == 0) {
        A355979_grow();
        A355979_P1[0] = 1;
        A355979_P2[0] = 1;
        A355979_len = 1;
        A355979_K = 1;
    }
    while (A355979_nf < (size_t)n) {
        A355979_step();
        if (A355979_is_prp(A355979_P1, A355979_len)) {
            if (A355979_nf == A355979_nfcap) {
                A355979_nfcap = A355979_nfcap ? A355979_nfcap * 2 : 64;
                A355979_found = (uint64_t *)realloc(A355979_found, A355979_nfcap * sizeof(uint64_t));
            }
            A355979_found[A355979_nf++] = A355979_K;
        }
    }
    return (seq_t)A355979_found[n - 1];
}
