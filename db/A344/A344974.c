/*
 * A344974  Numbers k such that A011772(k) divides A344875(k).
 * https://oeis.org/A344974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A011772(k) divides A344875(k);
 *            computed by direct search over k with cached terms
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A011772_small(uint64_t n)
{
    uint64_t limit = n * 2u;
    for (uint64_t m = 1; m < limit; ++m) {
        unsigned __int128 prod = (unsigned __int128)m * (m + 1) / 2;
        if ((prod % n) == 0)
            return m;
    }
    return limit - 1;
}

static seq_t A344875_val(uint64_t n)
{
    seq_t ans = 1;
    uint64_t tmp = n;
    while (tmp > 1) {
        uint64_t p = 0, e = 0;
        if (tmp % 2 == 0) {
            p = 2;
        } else {
            for (uint64_t d = 3; d * d <= tmp; d += 2) {
                if (tmp % d == 0) { p = d; break; }
            }
            if (p == 0) p = tmp;
        }
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        seq_t term;
        if (p == 2) {
            seq_t pw = 1;
            for (uint64_t i = 0; i < e + 1; ++i) pw *= 2;
            term = pw - 1;
        } else {
            seq_t pw = 1;
            for (uint64_t i = 0; i < e; ++i) pw *= (seq_t)p;
            term = pw - 1;
        }
        ans *= term;
    }
    return ans;
}

static uint64_t *A344974_terms = 0;
static uint64_t A344974_count = 0;
static uint64_t A344974_cap = 0;
static uint64_t A344974_next_k = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A344974_count < (uint64_t)n) {
        uint64_t k = A344974_next_k++;
        uint64_t m = A011772_small(k);
        seq_t v = A344875_val(k);
        if ((v % (seq_t)m) == 0) {
            if (A344974_count == A344974_cap) {
                uint64_t nc = A344974_cap ? A344974_cap * 2 : 1024;
                A344974_terms = (uint64_t *)realloc(A344974_terms, nc * sizeof(uint64_t));
                A344974_cap = nc;
            }
            A344974_terms[A344974_count++] = k;
        }
    }
    return (seq_t)A344974_terms[n - 1];
}
