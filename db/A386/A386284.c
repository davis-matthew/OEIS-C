/*
 * A386284  Smallest k for which A386930(k) = n.
 * https://oeis.org/A386284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with A386930(k) = n, where A386930(k) = number
 *            of divisors d of k with (-d)^d == -d^d (mod k); found by
 *            incremental search over k with a smallest-prime-factor sieve
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define A386284_TAB 512

static uint32_t *A386284_spf = 0;
static uint64_t A386284_spfN = 0;
static uint64_t A386284_found[A386284_TAB];
static uint64_t A386284_cur = 0;

static uint64_t A386284_P[24];
static unsigned A386284_A[24];
static unsigned A386284_B[24];
static size_t A386284_np;
static unsigned A386284_e;
static uint64_t A386284_cnt;

static void A386284_ensure(uint64_t k)
{
    if (k <= A386284_spfN) return;
    uint64_t newN = A386284_spfN * 2;
    if (newN < (1ULL << 20)) newN = (1ULL << 20);
    if (newN < k) newN = k;
    if (A386284_spf) free(A386284_spf);
    A386284_spf = (uint32_t *)calloc((size_t)(newN + 1), sizeof(uint32_t));
    A386284_spfN = newN;
    for (uint64_t i = 2; i <= newN; ++i) {
        if (A386284_spf[i] == 0) {
            for (uint64_t j = i; j <= newN; j += i)
                if (A386284_spf[j] == 0) A386284_spf[j] = (uint32_t)i;
        }
    }
}

static void A386284_rec(size_t i, uint64_t t)
{
    if (i == A386284_np) {
        for (unsigned j = 1; j <= A386284_e; ++j) {
            uint64_t d = t << j;
            int ok = 1;
            for (size_t q = 0; q < A386284_np; ++q) {
                if (d * A386284_B[q] < A386284_A[q]) { ok = 0; break; }
            }
            if (ok && (uint64_t)A386284_e <= 1 + d * (uint64_t)j)
                A386284_cnt++;
        }
        return;
    }
    uint64_t tt = t;
    for (unsigned b = 1; b <= A386284_A[i]; ++b) {
        tt *= A386284_P[i];
        A386284_B[i] = b;
        A386284_rec(i + 1, tt);
    }
}

static uint64_t A386284_count(uint64_t k)
{
    unsigned e = 0;
    uint64_t m = k;
    while ((m & 1ULL) == 0) { m >>= 1; ++e; }
    size_t np = 0;
    uint64_t tau = 1;
    while (m > 1) {
        uint64_t p = A386284_spf[m];
        unsigned a = 0;
        while (m % p == 0) { m /= p; ++a; }
        A386284_P[np] = p;
        A386284_A[np] = a;
        ++np;
        tau *= (a + 1);
    }
    A386284_cnt = tau;
    if (e >= 1) {
        A386284_np = np;
        A386284_e = e;
        A386284_rec(0, 1);
    }
    return A386284_cnt;
}

seq_t a(idx_t n)
{
    if (n >= A386284_TAB) return 0;
    while (A386284_found[n] == 0) {
        uint64_t k = ++A386284_cur;
        A386284_ensure(k);
        uint64_t c = A386284_count(k);
        if (c < A386284_TAB && A386284_found[c] == 0)
            A386284_found[c] = k;
    }
    return (seq_t)A386284_found[n];
}
