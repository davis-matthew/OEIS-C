/*
 * A334872  Number of steps needed to reach either 1 or one of the
 *          "Fermi-Dirac primes" (A050376) when starting from n and iterating
 *          with A334870.
 * https://oeis.org/A334872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   write n = prod p_i^e_i (p_i = i-th prime, index from 0) and
 *            split e_i = 2*h_i + b_i with b_i in {0,1}. A334870 never changes
 *            the h_i while n is not a square. Each step just lowers B = Sum
 *            b_i*2^i by 1. When B reaches 0, n is a square and the next step
 *            replaces e_i by h_i. So each phase adds B ste...
 * Range:     u8 n <= 91, u16 n <= 235, u32 n <= 547, u64 n <= 1251,
 *            u128 n <= 2907, big any n
 * Validated: standard, pass, 121 terms through n = 9941 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A334872_primes = NULL;
static size_t A334872_pcnt = 0;
static uint64_t A334872_plimit = 0;

static void A334872_ensure_primes(uint64_t limit)
{
    char *comp;
    uint64_t i, j;
    size_t cnt = 0;
    if (limit < 2) limit = 2;
    if (limit <= A334872_plimit) return;
    if (limit < 2 * A334872_plimit) limit = 2 * A334872_plimit;
    comp = (char *)calloc((size_t)limit + 1, 1);
    for (i = 2; i <= limit; i++)
        if (!comp[i]) { cnt++; for (j = i * i; j <= limit; j += i) comp[j] = 1; }
    free(A334872_primes);
    A334872_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    cnt = 0;
    for (i = 2; i <= limit; i++) if (!comp[i]) A334872_primes[cnt++] = (uint32_t)i;
    free(comp);
    A334872_pcnt = cnt;
    A334872_plimit = limit;
}

/* 2^k computed in seq_t by repeated doubling (wraps harmlessly for fixed width) */
static seq_t A334872_p2(size_t k)
{
    seq_t r = (seq_t)1;
    while (k--) r = r * 2;
    return r;
}

seq_t a(idx_t n)
{
    size_t idx[64];
    uint64_t ex[64];
    size_t cnt = 0, i;
    uint64_t m = (uint64_t)n;
    seq_t steps = (seq_t)0;

    if (n <= 1) return (seq_t)0;
    A334872_ensure_primes(m);
    for (i = 0; i < A334872_pcnt && m > 1; i++) {
        uint64_t p = A334872_primes[i];
        if (p * p > m) break;
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            idx[cnt] = i; ex[cnt] = e; cnt++;
        }
    }
    if (m > 1) {
        /* m is a prime; find its index */
        size_t lo = 0, hi = A334872_pcnt;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if ((uint64_t)A334872_primes[mid] < m) lo = mid + 1; else hi = mid;
        }
        idx[cnt] = lo; ex[cnt] = 1; cnt++;
    }

    for (;;) {
        seq_t B = (seq_t)0;
        size_t hcount = 0, hk = 0, topodd = 0;
        bool anyodd = false;
        size_t w;

        if (cnt == 0) return steps;
        for (i = 0; i < cnt; i++) {
            if (ex[i] & 1) {
                B += A334872_p2(idx[i]);
                if (!anyodd || idx[i] > topodd) topodd = idx[i];
                anyodd = true;
            }
            if (ex[i] >= 2) { hcount++; hk = i; }
        }
        if (hcount == 0) {
            /* all exponents are 1: stop at highest power of 2 <= B */
            steps += B - A334872_p2(topodd);
            return steps;
        }
        if (hcount == 1) {
            uint64_t h = ex[hk] / 2;
            if ((h & (h - 1)) == 0) {
                steps += B;
                return steps;
            }
        }
        steps += B + 1;
        w = 0;
        for (i = 0; i < cnt; i++) {
            uint64_t h = ex[i] / 2;
            if (h) { idx[w] = idx[i]; ex[w] = h; w++; }
        }
        cnt = w;
    }
}
