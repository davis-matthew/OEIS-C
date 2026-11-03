/*
 * A332042  Number of integers whose Dedekind psi function (A001615) values
 *          are n.
 * https://oeis.org/A332042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9483 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts integers m with Dedekind ψ(m) = n
 * Dedekind ψ(m) = m × Π_{p | m} (1 + 1/p)
 * For an index n ≥ 1 the function returns the number of m ≥ 1 with ψ(m)=n. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* compute ψ(m) using trial division */
static idx_t psi_value(idx_t m)
{
    idx_t val  = m;
    idx_t x    = m;
    idx_t last = 0;

    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            if (p != last)
            {
                val = val * (p + 1) / p;
                last = p;
            }
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1 && x != last)      /* remaining prime factor */
    {
        val = val * (x + 1) / x;
    }
    return val;
}

/* static state */
static seq_t *cnt_arr = NULL;          /* counts of each ψ value */
static size_t  cnt_cap = 0;            /* current capacity of cnt_arr */
static idx_t   max_m   = 0;            /* maximum m processed so far */

/* enlarge count array to at least 'needed' elements, zero‑initialising new slots */
static void ensure_count_cap(size_t needed)
{
    if (needed <= cnt_cap) return;
    size_t old_cap = cnt_cap;
    size_t new_cap = old_cap ? old_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    seq_t *tmp = mem_resize(seq_t, cnt_arr, new_cap);
    if (!tmp) exit(1); /* out of memory */

    for (size_t i = old_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    cnt_arr = tmp;
    cnt_cap = new_cap;
}

/* extend precomputation up to at least m = n */
static void extend_counts(idx_t n)
{
    if (n <= max_m) return;
    for (idx_t m = max_m + 1; m <= n; ++m)
    {
        idx_t psi = psi_value(m);
        size_t idx = (size_t)psi;
        ensure_count_cap(idx + 1);
        cnt_arr[idx] += (seq_t)1;
    }
    max_m = n;
}

/* public sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    extend_counts(n);
    size_t idx = (size_t)n;
    return (idx < cnt_cap) ? cnt_arr[idx] : (seq_t)0;
}
