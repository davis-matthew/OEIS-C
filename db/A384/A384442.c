/*
 * A384442  Smallest k such that A361373(k) = n.
 * https://oeis.org/A384442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A361373(k) = n, where A361373(k) = Sum_{p |
 *            k} floor(log k / log p); found by incremental search with a
 *            smallest-prime-factor sieve
 * Range:     u8 n <= 14, u16 n <= 40, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A384442_MAXV 512

static uint32_t *A384442_spf = NULL;
static uint64_t A384442_L = 0;
static uint64_t A384442_next = 1;
static uint64_t A384442_first[A384442_MAXV];

static void A384442_build(uint64_t L)
{
    if (A384442_spf) free(A384442_spf);
    A384442_spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    A384442_L = L;
    for (uint64_t i = 2; i <= L; ++i) {
        if (A384442_spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (A384442_spf[j] == 0)
                    A384442_spf[j] = (uint32_t)i;
        }
    }
}

static unsigned A384442_f(uint64_t k)
{
    unsigned res = 0;
    uint64_t t = k;
    while (t > 1) {
        uint64_t p = A384442_spf[t];
        while (t % p == 0)
            t /= p;
        uint64_t v = p;
        unsigned c = 1;
        while (v <= k / p) {
            v *= p;
            ++c;
        }
        res += c;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n >= A384442_MAXV)
        return (seq_t)0;
    if (A384442_first[n] == 0) {
        while (A384442_first[n] == 0) {
            if (A384442_next > A384442_L)
                A384442_build(A384442_L ? A384442_L * 4 : ((uint64_t)1 << 16));
            uint64_t end = A384442_L;
            for (; A384442_next <= end; ++A384442_next) {
                unsigned v = (A384442_next == 1) ? 0 : A384442_f(A384442_next);
                if (v < A384442_MAXV && A384442_first[v] == 0)
                    A384442_first[v] = A384442_next;
                if (v == n && A384442_first[n] != 0) {
                    ++A384442_next;
                    break;
                }
            }
        }
    }
    return (seq_t)A384442_first[n];
}
