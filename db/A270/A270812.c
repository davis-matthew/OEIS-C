/*
 * A270812  Where records occur in A266569.
 * https://oeis.org/A270812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th (strict) left-to-right maximum of
 *            A266569, where A266569(1) = 1, A266569(2k) = 4k + A266569(k),
 *            A266569(2k+1) = k + A266569(4k+4)
 * Range:     u8 n <= 7, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 861 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A266569_cache = NULL;
static bool *A266569_computed = NULL;
static idx_t A266569_cap = 0;

static void A266569_ensure(idx_t n)
{
    if (n + 1 <= A266569_cap) return;
    idx_t new_cap = A266569_cap ? A266569_cap << 1 : 64;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *nc = mem_new(seq_t, new_cap);
    bool *nf = (bool *)malloc(new_cap * sizeof(bool));
    if (!nc || !nf) abort();
    for (idx_t i = 0; i < A266569_cap; ++i) {
        nc[i] = A266569_cache[i];
        nf[i] = A266569_computed[i];
    }
    for (idx_t i = A266569_cap; i < new_cap; ++i) {
        nc[i] = (seq_t)0;
        nf[i] = false;
    }
    if (A266569_cache) mem_free(A266569_cache);
    free(A266569_computed);
    A266569_cache = nc;
    A266569_computed = nf;
    A266569_cap = new_cap;
}

static seq_t A266569_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A266569_ensure(n);
    if (A266569_computed[n]) return A266569_cache[n];
    if (n == 1) {
        A266569_cache[1] = (seq_t)1;
        A266569_computed[1] = true;
        return A266569_cache[1];
    }
    seq_t res;
    if ((n & 1) == 0) {
        idx_t k = n >> 1;
        res = ((seq_t)4 * (seq_t)k) + A266569_a(k);
    } else {
        idx_t k = (n - 1) >> 1;
        idx_t m = 4 * k + 4;
        res = (seq_t)k + A266569_a(m);
    }
    A266569_ensure(n);
    A266569_cache[n] = res;
    A266569_computed[n] = true;
    return res;
}

static idx_t *rec_pos = NULL;
static idx_t rec_count = 0;
static idx_t rec_cap = 0;
static idx_t scanned = 0;
static seq_t cur_max = 0;

seq_t a(idx_t n)
{
    while (rec_count < n) {
        idx_t m = ++scanned;
        seq_t v = A266569_a(m);
        if (m == 1 || v > cur_max) {
            cur_max = v;
            if (rec_count == rec_cap) {
                idx_t nc = rec_cap ? rec_cap * 2 : 64;
                idx_t *np = (idx_t *)realloc(rec_pos, nc * sizeof(idx_t));
                if (!np) abort();
                rec_pos = np;
                rec_cap = nc;
            }
            rec_pos[rec_count++] = m;
        }
    }
    return (seq_t)rec_pos[n - 1];
}
