/*
 * A304738  Restricted growth sequence transform of A278222(A048673(n)).
 * https://oeis.org/A304738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of
 *            A278222(A048673(n)); labels are assigned in order of first
 *            appearance of each multiset of binary 1-run lengths of
 *            (A003961(n)+1)/2 (computed by sequential generation with
 *            memoization)
 * Range:     u8 n <= 16604, u16 n <= 65539, u32 n <= 65539, u64 n <= 65539,
 *            u128 n <= 65539, big any n
 * Validated: standard, pass, 121 terms through n = 9559 (2026-11-08)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

typedef struct {
    size_t len;
    uint8_t *lens;
    unsigned int label;
} A304738_Key;

static unsigned int *A304738_labels = NULL;
static size_t A304738_cap = 0;
static uint64_t A304738_gen = 0;
static A304738_Key *A304738_keys = NULL;
static size_t A304738_kcnt = 0, A304738_kcap = 0;
static unsigned int A304738_next = 1;

static uint64_t A304738_A048673(uint64_t n)
{
    if (n == 1)
        return 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod *= np;
    }
    return (prod + 1) / 2;
}

static size_t A304738_runs(uint64_t m, uint8_t *out)
{
    size_t len = 0;
    while (m) {
        while ((m & 1ULL) == 0)
            m >>= 1;
        uint8_t c = 0;
        while (m & 1ULL) {
            ++c;
            m >>= 1;
        }
        out[len++] = c;
    }
    for (size_t i = 1; i < len; ++i) {
        uint8_t v = out[i];
        size_t j = i;
        while (j > 0 && out[j - 1] > v) {
            out[j] = out[j - 1];
            --j;
        }
        out[j] = v;
    }
    return len;
}

static unsigned int A304738_label_of(uint64_t m)
{
    uint8_t tmp[64];
    size_t len = A304738_runs(m, tmp);
    for (size_t k = 0; k < A304738_kcnt; ++k) {
        if (A304738_keys[k].len == len &&
            (len == 0 || memcmp(A304738_keys[k].lens, tmp, len) == 0))
            return A304738_keys[k].label;
    }
    if (A304738_kcnt >= A304738_kcap) {
        size_t nc = A304738_kcap ? A304738_kcap * 2 : 16;
        A304738_Key *p = (A304738_Key *)realloc(A304738_keys, nc * sizeof(A304738_Key));
        if (!p) abort();
        A304738_keys = p;
        A304738_kcap = nc;
    }
    A304738_Key *kp = &A304738_keys[A304738_kcnt++];
    kp->len = len;
    kp->lens = (uint8_t *)malloc(len ? len : 1);
    if (!kp->lens) abort();
    if (len) memcpy(kp->lens, tmp, len);
    kp->label = A304738_next++;
    return kp->label;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N >= A304738_cap) {
        size_t nc = A304738_cap ? A304738_cap : 64;
        while (nc <= N) nc *= 2;
        unsigned int *p = (unsigned int *)realloc(A304738_labels, nc * sizeof(unsigned int));
        if (!p) abort();
        A304738_labels = p;
        A304738_cap = nc;
    }
    for (uint64_t i = A304738_gen + 1; i <= N; ++i)
        A304738_labels[i] = A304738_label_of(A304738_A048673(i));
    if (N > A304738_gen)
        A304738_gen = N;
    return (seq_t)A304738_labels[N];
}
