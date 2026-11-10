/*
 * A378604  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A033630(i) = A033630(j) and A083206(i) = A083206(j), for all i,
 *          j >= 1.
 * https://oeis.org/A378604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A033630(n), A083206(n)]; computed incrementally by DP for both
 *            sequences with first-appearance numbering
 * Range:     u8 n <= 1799, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9939 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t *A378604_dp = NULL;
static size_t A378604_dpcap = 0;
static uint32_t *A378604_ids = NULL;
static size_t A378604_idcap = 0;
static size_t A378604_done = 0;
static uint64_t *A378604_ra = NULL;
static uint64_t *A378604_rb = NULL;
static size_t A378604_nreps = 0;
static size_t A378604_repcap = 0;

static void A378604_pair(uint64_t m, uint64_t *p1, uint64_t *p2)
{
    uint64_t divs[512];
    size_t dc = 0;
    uint64_t sigma = 0;
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            divs[dc++] = i;
            sigma += i;
            if (i != m / i) {
                divs[dc++] = m / i;
                sigma += m / i;
            }
        }
    }
    size_t need = (size_t)m + 1;
    if ((size_t)(sigma / 2) + 1 > need) need = (size_t)(sigma / 2) + 1;
    if (A378604_dpcap < need) {
        free(A378604_dp);
        A378604_dpcap = need * 2;
        A378604_dp = (uint64_t *)malloc(A378604_dpcap * sizeof(uint64_t));
    }
    uint64_t *dp = A378604_dp;
    /* A033630: distinct divisor partitions of m */
    for (size_t s = 0; s <= (size_t)m; ++s) dp[s] = 0;
    dp[0] = 1;
    for (size_t k = 0; k < dc; ++k) {
        size_t d = (size_t)divs[k];
        if (d >= m) continue;
        for (size_t s = (size_t)m; s >= d; --s) dp[s] += dp[s - d];
    }
    *p1 = dp[m] + 1;
    /* A083206 */
    if (sigma & 1) {
        *p2 = 0;
    } else {
        size_t t = (size_t)(sigma / 2);
        for (size_t s = 0; s <= t; ++s) dp[s] = 0;
        dp[0] = 1;
        for (size_t k = 0; k < dc; ++k) {
            size_t d = (size_t)divs[k];
            for (size_t s = t; s >= d; --s) dp[s] += dp[s - d];
        }
        *p2 = dp[t] / 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A378604_idcap < (size_t)n + 1) {
        size_t nc = (size_t)n * 2 + 16;
        uint32_t *ni = (uint32_t *)malloc(nc * sizeof(uint32_t));
        for (size_t i = 0; i < A378604_idcap; ++i) ni[i] = A378604_ids[i];
        free(A378604_ids);
        A378604_ids = ni;
        A378604_idcap = nc;
    }
    while (A378604_done < (size_t)n) {
        size_t m = A378604_done + 1;
        uint64_t x, y;
        A378604_pair((uint64_t)m, &x, &y);
        size_t found = 0;
        for (size_t i = 0; i < A378604_nreps; ++i) {
            if (A378604_ra[i] == x && A378604_rb[i] == y) { found = i + 1; break; }
        }
        if (!found) {
            if (A378604_nreps == A378604_repcap) {
                size_t nc = A378604_repcap ? A378604_repcap * 2 : 256;
                uint64_t *na = (uint64_t *)malloc(nc * sizeof(uint64_t));
                uint64_t *nb = (uint64_t *)malloc(nc * sizeof(uint64_t));
                for (size_t i = 0; i < A378604_nreps; ++i) {
                    na[i] = A378604_ra[i];
                    nb[i] = A378604_rb[i];
                }
                free(A378604_ra);
                free(A378604_rb);
                A378604_ra = na;
                A378604_rb = nb;
                A378604_repcap = nc;
            }
            A378604_ra[A378604_nreps] = x;
            A378604_rb[A378604_nreps] = y;
            ++A378604_nreps;
            found = A378604_nreps;
        }
        A378604_ids[m] = (uint32_t)found;
        A378604_done = m;
    }
    return (seq_t)A378604_ids[(size_t)n];
}
