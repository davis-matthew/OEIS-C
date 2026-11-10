/*
 * A333545  Indices k such that A217287(k) < A061836(k).
 * https://oeis.org/A333545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A217287(k) < A061836(k); computed by
 *            search: with L = A217287(k), the condition holds iff no j in
 *            1..L has (k+j) | j!
 * Range:     u8 n <= 134, u16 n <= 13371, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 92 terms through n = 8795 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static int A333545_fac(uint64_t m, uint64_t *pr, int *ex)
{
    int c = 0;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[c] = p; ex[c] = e; ++c;
        }
    }
    if (m > 1) { pr[c] = m; ex[c] = 1; ++c; }
    return c;
}

static bool A333545_isused(const uint64_t *used, int cnt, uint64_t p)
{
    for (int i = 0; i < cnt; ++i) if (used[i] == p) return true;
    return false;
}

static uint64_t A333545_chain(uint64_t k)
{
    uint64_t used[1024];
    int ucnt = 0;
    uint64_t pr[64];
    int ex[64];
    int c = A333545_fac(k, pr, ex);
    for (int i = 0; i < c; ++i)
        if (!A333545_isused(used, ucnt, pr[i]) && ucnt < 1024) used[ucnt++] = pr[i];
    uint64_t len = 1;
    uint64_t m = k;
    for (;;) {
        ++m;
        c = A333545_fac(m, pr, ex);
        bool has_new = false;
        for (int i = 0; i < c; ++i)
            if (!A333545_isused(used, ucnt, pr[i])) { has_new = true; break; }
        if (!has_new) break;
        for (int i = 0; i < c; ++i)
            if (!A333545_isused(used, ucnt, pr[i]) && ucnt < 1024) used[ucnt++] = pr[i];
        ++len;
    }
    return len;
}

static uint64_t A333545_leg(uint64_t j, uint64_t p)
{
    uint64_t e = 0;
    while (j) { j /= p; e += j; }
    return e;
}

/* true if some j in 1..L has (k+j) | j! */
static bool A333545_small_hit(uint64_t k, uint64_t L)
{
    uint64_t pr[64];
    int ex[64];
    for (uint64_t j = 1; j <= L; ++j) {
        int c = A333545_fac(k + j, pr, ex);
        bool ok = true;
        for (int i = 0; i < c; ++i) {
            if (A333545_leg(j, pr[i]) < (uint64_t)ex[i]) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

static uint64_t *A333545_list = NULL;
static size_t A333545_cnt = 0, A333545_cap = 0;
static uint64_t A333545_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A333545_cnt < (size_t)n) {
        uint64_t k = A333545_next++;
        uint64_t L = A333545_chain(k);
        if (!A333545_small_hit(k, L)) {
            if (A333545_cnt == A333545_cap) {
                A333545_cap = A333545_cap ? A333545_cap * 2 : 1024;
                A333545_list = (uint64_t *)realloc(A333545_list, A333545_cap * sizeof(uint64_t));
            }
            A333545_list[A333545_cnt++] = k;
        }
    }
    return (seq_t)A333545_list[n - 1];
}
