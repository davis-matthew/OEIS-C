/*
 * A351337  Composite terms in A270951.
 * https://oeis.org/A351337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite k such that k | A000129(k-1) (Pell
 *            numbers); found by direct search with modular matrix power
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t A351337_mm(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

/* Pell(n) mod m, via powers of [[2,1],[1,0]] kept as symmetric (x,y,z) = [[x,y],[y,z]] */
static uint64_t A351337_pell(uint64_t n, uint64_t m)
{
    if (m == 1) return 0;
    uint64_t ra = 1, rb = 0, rc = 1;
    uint64_t ma = 2 % m, mb = 1 % m, mc = 0;
    while (n) {
        if (n & 1) {
            uint64_t na = (A351337_mm(ra, ma, m) + A351337_mm(rb, mb, m)) % m;
            uint64_t nb = (A351337_mm(ra, mb, m) + A351337_mm(rb, mc, m)) % m;
            uint64_t nc = (A351337_mm(rb, mb, m) + A351337_mm(rc, mc, m)) % m;
            ra = na; rb = nb; rc = nc;
        }
        uint64_t qa = (A351337_mm(ma, ma, m) + A351337_mm(mb, mb, m)) % m;
        uint64_t qb = (A351337_mm(ma, mb, m) + A351337_mm(mb, mc, m)) % m;
        uint64_t qc = (A351337_mm(mb, mb, m) + A351337_mm(mc, mc, m)) % m;
        ma = qa; mb = qb; mc = qc;
        n >>= 1;
    }
    return rb % m;
}

static bool A351337_composite(uint64_t k)
{
    if (k < 4) return false;
    if (k % 2 == 0) return true;
    for (uint64_t d = 3; d * d <= k; d += 2)
        if (k % d == 0) return true;
    return false;
}

static seq_t *A351337_cache = NULL;
static size_t A351337_cap = 0;
static idx_t A351337_len = 0;
static uint64_t A351337_next = 4;

seq_t a(idx_t n)
{
    while (A351337_len < n) {
        uint64_t k = A351337_next++;
        if (A351337_pell(k - 1, k) == 0 && A351337_composite(k)) {
            if ((size_t)A351337_len + 1 > A351337_cap) {
                size_t nc = A351337_cap ? A351337_cap * 2 : 64;
                seq_t *t = mem_resize(seq_t, A351337_cache, nc);
                A351337_cache = t;
                A351337_cap = nc;
            }
            A351337_cache[A351337_len++] = (seq_t)k;
        }
    }
    return A351337_cache[n - 1];
}
