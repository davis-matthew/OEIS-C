/*
 * A349428  Smallest k such that A349410(k) = n or -1 if no such number
 *          exists.
 * https://oeis.org/A349428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = min{k : A349410(k) = n}, found by searching k in
 *            increasing bounds L = 2^16, 2^17, ... Only canonical k are
 *            evaluated: primes > 43 never divide a trajectory value, so in a
 *            minimal k they are consecutive from 47 with non-increasing
 *            exponents. For each candidate the cycle length of x -> d(k*...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define A349428_NP 30
#define A349428_SMALL 14            /* primes 2..43 */
#define A349428_MAXN 128
#define A349428_SPF 65536u
#define A349428_CAP (1ULL << 36)

static const uint32_t A349428_PR[A349428_NP] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,
    47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113 };
static int A349428_idxof[128];
static uint16_t A349428_spf[A349428_SPF];
static uint32_t A349428_plist[7000];
static int A349428_np = 0;
static bool A349428_init = false;

static uint64_t A349428_best[A349428_MAXN];
static uint64_t A349428_done = 0;     /* all canonical k <= done evaluated */
static uint64_t A349428_L = 0;

static uint32_t A349428_exp[A349428_NP];
static uint64_t A349428_dk;

static void A349428_setup(void)
{
    uint32_t i, j;
    if (A349428_init) return;
    for (i = 0; i < A349428_SPF; i++) A349428_spf[i] = 0;
    for (i = 2; i < A349428_SPF; i++) {
        if (A349428_spf[i] == 0) {
            A349428_plist[A349428_np++] = i;
            for (j = i; j < A349428_SPF; j += i)
                if (A349428_spf[j] == 0) A349428_spf[j] = (uint16_t)i;
        }
    }
    for (i = 0; i < 128; i++) A349428_idxof[i] = -1;
    for (i = 0; i < A349428_NP; i++) A349428_idxof[A349428_PR[i]] = (int)i;
    for (i = 0; i < A349428_MAXN; i++) A349428_best[i] = 0;
    for (i = 0; i < A349428_NP; i++) A349428_exp[i] = 0;
    A349428_init = true;
}

/* d(k*y) using the current exponent vector of k */
static uint64_t A349428_f(uint64_t y)
{
    uint64_t res = A349428_dk;
    while (y > 1) {
        uint64_t q, e = 0, ek = 0;
        if (y < A349428_SPF) {
            q = A349428_spf[y];
        } else {
            int i;
            q = 0;
            for (i = 0; i < A349428_np; i++) {
                uint64_t p = A349428_plist[i];
                if (p * p > y) break;
                if (y % p == 0) { q = p; break; }
            }
            if (q == 0) q = y;
        }
        while (y % q == 0) { y /= q; e++; }
        if (q < 128 && A349428_idxof[q] >= 0)
            ek = A349428_exp[A349428_idxof[q]];
        res = res / (ek + 1) * (ek + e + 1);
    }
    return res;
}

static uint64_t A349428_cycle(void)
{
    uint64_t power = 1, lam = 1;
    uint64_t t = 1;
    uint64_t h = A349428_f(1);
    while (t != h) {
        if (power == lam) { t = h; power *= 2; lam = 0; }
        h = A349428_f(h);
        lam++;
    }
    return lam;
}

static void A349428_rec(int i, uint64_t k, uint64_t dk)
{
    int j;
    uint64_t lam;
    A349428_dk = dk;
    lam = A349428_cycle();
    if (lam < A349428_MAXN && (A349428_best[lam] == 0 || k < A349428_best[lam]))
        A349428_best[lam] = k;
    for (j = i; j < A349428_NP; j++) {
        uint64_t p = A349428_PR[j];
        uint32_t maxe = 200, e;
        uint64_t kk = k;
        if (j > A349428_SMALL) {
            if (j != i) break;
            maxe = A349428_exp[j - 1];
        }
        if (k > A349428_L / p) {
            if (j >= A349428_SMALL) break;
            continue;
        }
        for (e = 1; e <= maxe && kk <= A349428_L / p; e++) {
            kk *= p;
            A349428_exp[j] = e;
            A349428_rec(j + 1, kk, dk * (e + 1));
        }
        A349428_exp[j] = 0;
    }
}

seq_t a(idx_t n)
{
    seq_t r;
    A349428_setup();
    if (n >= 1 && n < A349428_MAXN) {
        while (A349428_best[n] == 0 && A349428_done < A349428_CAP) {
            A349428_L = (A349428_done == 0) ? (1ULL << 16) : A349428_done * 2;
            A349428_rec(0, 1, 1);
            A349428_done = A349428_L;
        }
        if (A349428_best[n] != 0)
            return (seq_t)A349428_best[n];
    }
    r = 0;
    r = r - 1;
    return r;
}
