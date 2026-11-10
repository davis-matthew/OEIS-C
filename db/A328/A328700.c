/*
 * A328700  Numbers k dividing nonzero terms in A003095.
 * https://oeis.org/A328700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve. k is in the sequence iff, for each prime power p^e
 *            dividing k exactly, the orbit of x -> x^2+1 (mod p^e) starting
 *            at 0 comes back to 0. These are the k with A328699(k) = 0. If
 *            p^e fails, every multiple of p^e is excluded. Primes p == 3 (mod
 *            4) always fail, because -1 is not a square mod p
 * Range:     u8 n <= 14, u16 n <= 311, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 996 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool A328700_pure(uint64_t m) {
    uint64_t x = 0, saved = 0, power = 1, lam = 0;
    if (m == 1) return true;
    for (;;) {
        x = (x * x + 1) % m;
        if (x == 0) return true;
        if (x == saved) return false;
        lam++;
        if (lam == power) { saved = x; power <<= 1; lam = 0; }
    }
}

static uint64_t *A328700_terms = NULL;
static size_t A328700_cnt = 0;
static uint64_t A328700_limit = 0;

static void A328700_build(uint64_t L) {
    unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
    unsigned char *bad = (unsigned char *)calloc(L + 1, 1);
    uint64_t p, m, i;
    for (p = 2; p <= L; p++) {
        uint64_t pe;
        bool fail = false;
        if (comp[p]) continue;
        if (p <= L / p)
            for (m = p * p; m <= L; m += p) comp[m] = 1;
        pe = p;
        if (p % 4 == 3) fail = true;
        else {
            for (;;) {
                if (!A328700_pure(pe)) { fail = true; break; }
                if (pe > L / p) break;
                pe *= p;
            }
        }
        if (fail) for (m = pe; m <= L; m += pe) bad[m] = 1;
    }
    free(A328700_terms);
    A328700_terms = (uint64_t *)malloc(sizeof(uint64_t) * (L + 1));
    A328700_cnt = 0;
    for (i = 1; i <= L; i++)
        if (!bad[i]) A328700_terms[A328700_cnt++] = i;
    A328700_limit = L;
    free(comp);
    free(bad);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (A328700_limit == 0) A328700_build(4096);
    while ((uint64_t)n > A328700_cnt) A328700_build(A328700_limit * 2);
    return (seq_t)A328700_terms[n - 1];
}
