/*
 * A364294  Difference k - A163511(k) computed for those odd numbers k for
 *          which the difference is nonnegative.
 * https://oeis.org/A364294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = -A364258(A364293(n)); search over odd k = 1, 3, 5, ...,
 *            keeping k - A163511(k) whenever A163511(k) <= k, where
 *            A163511(k) is computed from the binary positions of the 1-bits
 *            of k
 * Range:     u8 n <= 25, u16 n <= 766, u32 n <= 15157, u64 n <= 15157,
 *            u128 n <= 15157, big any n
 * Validated: standard, pass, 83 terms through n = 9847 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint32_t A364294_pr[65];
static int A364294_pr_init = 0;

static void A364294_init_primes(void)
{
    int c = 0;
    uint32_t x = 2;
    while (c < 65) {
        int ok = 1;
        for (uint32_t d = 2; d * d <= x; d++) {
            if (x % d == 0) { ok = 0; break; }
        }
        if (ok) A364294_pr[c++] = x;
        x++;
    }
    A364294_pr_init = 1;
}

/* If A163511(k) <= k, returns true and sets *diff = k - A163511(k). */
static bool A364294_diff(idx_t k, uint64_t *diff)
{
    unsigned __int128 kk = (unsigned __int128)k;
    idx_t pos[64];
    int cnt = 0;
    idx_t bitpos = 1;
    idx_t tmp = k;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        bitpos++;
        tmp >>= 1;
    }
    unsigned __int128 result = (unsigned __int128)A364294_pr[cnt - 1];
    if (result > kk) return false;
    idx_t prev = 0;
    for (int i = 0; i < cnt; i++) {
        idx_t curr = pos[i];
        idx_t ex = curr - prev - 1;
        unsigned __int128 base = (unsigned __int128)A364294_pr[i];
        for (idx_t e = 0; e < ex; e++) {
            result *= base;
            if (result > kk) return false;
        }
        prev = curr;
    }
    *diff = (uint64_t)(kk - result);
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *vals = NULL;
    static size_t cnt = 0, cap = 0;
    static idx_t next_k = 1;
    if (!A364294_pr_init) A364294_init_primes();
    if (vals == NULL) {
        cap = 1024;
        vals = (uint64_t *)malloc(cap * sizeof(uint64_t));
    }
    while (cnt < (size_t)n) {
        uint64_t d;
        if (A364294_diff(next_k, &d)) {
            if (cnt == cap) {
                cap *= 2;
                vals = (uint64_t *)realloc(vals, cap * sizeof(uint64_t));
            }
            vals[cnt++] = d;
        }
        next_k += 2;
    }
    return (seq_t)vals[n - 1];
}
