/*
 * A357390  Numbers k such that A071626(k) > A071626(k+1).
 * https://oeis.org/A357390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A071626(k) > A071626(k+1); A071626(k) =
 *            number of distinct exponents in k!, computed by an incremental
 *            scan with a smallest-prime-factor sieve
 * Range:     u8 n <= 6, u16 n <= 2132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9745 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static size_t   A357390_L = 0;
static uint32_t *A357390_spf = NULL;
static uint32_t *A357390_e = NULL;
static uint32_t *A357390_cnt = NULL;
static uint32_t *A357390_terms = NULL;
static size_t   A357390_nterms = 0, A357390_cap = 0;
static size_t   A357390_k = 1;
static long long A357390_D = 0;

static void A357390_rebuild(size_t L)
{
    free(A357390_spf); free(A357390_e); free(A357390_cnt);
    A357390_L = L;
    A357390_spf = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    A357390_e   = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    A357390_cnt = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    if (!A357390_spf || !A357390_e || !A357390_cnt) exit(EXIT_FAILURE);
    for (size_t i = 2; i <= L; ++i) {
        if (A357390_spf[i] == 0) {
            for (size_t m = i; m <= L; m += i)
                if (A357390_spf[m] == 0) A357390_spf[m] = (uint32_t)i;
        }
    }
    A357390_nterms = 0;
    A357390_k = 1;
    A357390_D = 0;
}

static void A357390_step(void)
{
    size_t x = A357390_k + 1;
    long long Dk = A357390_D;
    while (x > 1) {
        uint32_t p = A357390_spf[x];
        uint32_t v = 0;
        while (x % p == 0) { x /= p; ++v; }
        uint32_t old = A357390_e[p];
        if (old) {
            if (--A357390_cnt[old] == 0) A357390_D--;
        }
        A357390_e[p] = old + v;
        if (A357390_cnt[old + v]++ == 0) A357390_D++;
    }
    if (Dk > A357390_D) {
        if (A357390_nterms == A357390_cap) {
            A357390_cap = A357390_cap ? A357390_cap * 2 : 1024;
            A357390_terms = (uint32_t *)realloc(A357390_terms, A357390_cap * sizeof(uint32_t));
            if (!A357390_terms) exit(EXIT_FAILURE);
        }
        A357390_terms[A357390_nterms++] = (uint32_t)A357390_k;
    }
    A357390_k++;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A357390_L == 0) A357390_rebuild(1 << 16);
    while (A357390_nterms < (size_t)n) {
        if (A357390_k + 1 > A357390_L) A357390_rebuild(A357390_L * 2);
        A357390_step();
    }
    return (seq_t)A357390_terms[(size_t)n - 1];
}
