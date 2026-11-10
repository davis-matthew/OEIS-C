/*
 * A356992  Then a(n) = n - b(n - b(n - b(n - b(n - b(n - b(n)))))) for n >=
 *          2, where b(n) = A356988(n).
 * https://oeis.org/A356992
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - b(n - b(n - b(n - b(n - b(n - b(n)))))) for n >= 2,
 *            where b = A356988, b(1) = 1, b(n) = n - b(b(n - b(b(b(n-1)))))
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 82 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A356988_cache = NULL;
static size_t A356988_cap   = 0;
static idx_t  A356988_sz    = 0;

static void A356988_ensure_cap(size_t needed)
{
    if (A356988_cap >= needed) return;
    size_t newCap = A356988_cap ? A356988_cap << 1 : 1;
    while (newCap < needed) newCap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A356988_cache, newCap);
    if (!tmp) exit(1);
    A356988_cache = tmp;
    A356988_cap = newCap;
}

static seq_t A356988_a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n <= A356988_sz) return A356988_cache[n - 1];

    A356988_ensure_cap((size_t)n);

    for (idx_t i = A356988_sz + 1; i <= n; ++i)
    {
        if (i == 1) { A356988_cache[0] = 1; continue; }
        seq_t a_prev  = A356988_cache[i - 2];
        seq_t a2_prev = A356988_cache[(idx_t)a_prev - 1];
        seq_t a3_prev = A356988_cache[(idx_t)a2_prev - 1];
        seq_t m = (seq_t)i - a3_prev;
        idx_t idxM = (idx_t)m;
        seq_t a_m  = A356988_cache[idxM - 1];
        seq_t a2_m = A356988_cache[(idx_t)a_m - 1];
        A356988_cache[i - 1] = (seq_t)i - a2_m;
    }
    A356988_sz = n;
    return A356988_cache[n - 1];
}

seq_t a(idx_t n)
{
    seq_t t = A356988_a(n);
    for (int k = 0; k < 5; ++k)
        t = A356988_a((idx_t)((seq_t)n - t));
    return (seq_t)n - t;
}
