/*
 * A387634  Indices k such that s(k) is a prime p, s(k-1) = 2*p, and s(k+1) =
 *          3*p, where s = A386482.
 * https://oeis.org/A387634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. Generate s = A386482 term by term: s(1)=1, s(2)=2, then
 *            s(k) = largest unused j < s(k-1) with gcd(j, s(k-1)) > 1, or if
 *            none, the smallest unused j > s(k-1) with gcd(j, s(k-1)) > 1.
 *            a(n) = n-th index k with s(k) = p prime, s(k-1) = 2*p, s(k+1) =
 *            3*p
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define A387634_LOQ 65536u

static uint64_t *A387634_used = NULL;
static uint64_t A387634_used_bits = 0;
static uint16_t *A387634_spf = NULL;   /* odd-only smallest prime factor, 0 = prime */
static uint64_t A387634_spf_lim = 0;
static uint32_t A387634_lo[A387634_LOQ]; /* smallest multiplier m with m*q unused (0 = unset) */
static uint64_t A387634_cnt = 0;
static uint64_t A387634_p2 = 0, A387634_p1 = 0;
static uint64_t *A387634_found = NULL;
static size_t A387634_nf = 0, A387634_fcap = 0;

static void A387634_ensure_used(uint64_t idx)
{
    if (idx < A387634_used_bits) return;
    uint64_t nb = A387634_used_bits ? A387634_used_bits : 4096;
    while (nb <= idx) nb <<= 1;
    uint64_t *t = (uint64_t *)realloc(A387634_used, (size_t)(nb / 64) * sizeof(uint64_t));
    if (!t) abort();
    memset(t + A387634_used_bits / 64, 0,
           (size_t)((nb - A387634_used_bits) / 64) * sizeof(uint64_t));
    A387634_used = t;
    A387634_used_bits = nb;
}

static inline bool A387634_isused(uint64_t i)
{
    if (i >= A387634_used_bits) return false;
    return (A387634_used[i >> 6] >> (i & 63)) & 1;
}

static inline void A387634_setused(uint64_t i)
{
    A387634_ensure_used(i);
    A387634_used[i >> 6] |= (uint64_t)1 << (i & 63);
}

static void A387634_build_spf(uint64_t lim)
{
    free(A387634_spf);
    A387634_spf = (uint16_t *)calloc((size_t)(lim / 2) + 1, sizeof(uint16_t));
    if (!A387634_spf) abort();
    for (uint64_t i = 3; i * i <= lim; i += 2) {
        if (A387634_spf[i >> 1] == 0) {
            for (uint64_t j = i * i; j <= lim; j += 2 * i)
                if (A387634_spf[j >> 1] == 0) A387634_spf[j >> 1] = (uint16_t)i;
        }
    }
    A387634_spf_lim = lim;
}

static void A387634_step(void)
{
    if (A387634_cnt == 0) {
        A387634_setused(1); A387634_setused(2);
        A387634_p2 = 1; A387634_p1 = 2; A387634_cnt = 2;
        return;
    }
    uint64_t prev = A387634_p1;
    if (prev > A387634_spf_lim) {
        uint64_t l = A387634_spf_lim ? A387634_spf_lim * 2 : 65536;
        while (l < prev) l <<= 1;
        A387634_build_spf(l);
    }
    uint64_t qs[20]; int nq = 0;
    uint64_t m = prev;
    if ((m & 1) == 0) { qs[nq++] = 2; while ((m & 1) == 0) m >>= 1; }
    while (m > 1) {
        uint64_t p = A387634_spf[m >> 1] ? A387634_spf[m >> 1] : m;
        qs[nq++] = p;
        while (m % p == 0) m /= p;
    }
    bool prev_prime = (prev == 2) || ((prev & 1) && prev > 2 && A387634_spf[prev >> 1] == 0);

    uint64_t best = 0;
    for (int i = 0; i < nq; i++) {
        uint64_t q = qs[i];
        uint64_t lowm = 1;
        if (q < A387634_LOQ) {
            uint64_t l = A387634_lo[q] ? A387634_lo[q] : 1;
            while (A387634_isused(l * q)) l++;
            A387634_lo[q] = (uint32_t)l;
            lowm = l;
        }
        if (best / q + 1 > lowm) lowm = best / q + 1;
        uint64_t hi = (prev - 1) / q;
        for (uint64_t mm = hi; mm >= lowm; mm--) {
            if (!A387634_isused(mm * q)) { best = mm * q; break; }
        }
    }
    if (best == 0) {
        for (int i = 0; i < nq; i++) {
            uint64_t q = qs[i];
            uint64_t mm = prev / q + 1;
            if (q < A387634_LOQ && A387634_lo[q] > mm) mm = A387634_lo[q];
            for (; best == 0 || mm * q < best; mm++) {
                if (!A387634_isused(mm * q)) { best = mm * q; break; }
            }
        }
    }
    A387634_setused(best);
    A387634_cnt++;
    /* s(cnt-2)=p2, s(cnt-1)=prev, s(cnt)=best; k = cnt-1 */
    if (prev_prime && best == 3 * prev && A387634_p2 == 2 * prev) {
        if (A387634_nf == A387634_fcap) {
            A387634_fcap = A387634_fcap ? A387634_fcap * 2 : 64;
            A387634_found = (uint64_t *)realloc(A387634_found, A387634_fcap * sizeof(uint64_t));
            if (!A387634_found) abort();
        }
        A387634_found[A387634_nf++] = A387634_cnt - 1;
    }
    A387634_p2 = prev;
    A387634_p1 = best;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A387634_nf < (size_t)n) A387634_step();
    return (seq_t)A387634_found[(size_t)n - 1];
}
