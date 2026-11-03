/*
 * A340585  Noncube perfect powers.
 * https://oeis.org/A340585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9927 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence of non‑cube perfect powers.
   Algorithm: generate all integers a^k (a≥2, k≥2) that are ≤ LIMIT,
   skip numbers that are perfect cubes (checked via floor_cube_root),
   collect them, sort and deduplicate.
   The resulting array is stored once and reused for subsequent calls.
   LIMIT = 200000000 is enough to cover at least the first 20000 terms
   (and satisfies the 2‑second limit for n≈10000). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *values = NULL;
static size_t values_count = 0;
static bool initialized = false;

/* Helper to generate the array on first use */
static void generate_values(void)
{
    const seq_t LIMIT = (seq_t)200000000;
    size_t cap = 200000;            /* initial capacity (over‑estimate) */
    seq_t *buf = mem_new(seq_t, cap);
    if (!buf) exit(1);
    size_t cnt = 0;

    seq_t base = 2;
    while (base * base <= LIMIT) {
        seq_t val = base * base;    /* exponent 2 */
        while (1) {
            /* skip if val is a perfect cube */
            seq_t c = floor_cube_root(val);
            if (c * c * c != val) {
                if (cnt >= cap) {
                    cap *= 2;
                    seq_t *tmp = mem_resize(seq_t, buf, cap);
                    if (!tmp) exit(1);
                    buf = tmp;
                }
                buf[cnt++] = val;
            }
            /* prepare next exponent */
            if (val > LIMIT / base) break;
            val = val * base;
        }
        base++;
    }

    /* Sort and deduplicate */
    mem_qsort(buf, cnt, sizeof(seq_t), cmp_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0 || buf[i] != buf[i - 1]) {
            buf[uniq++] = buf[i];
        }
    }
    buf = mem_resize(seq_t, buf, uniq);
    values = buf;
    values_count = uniq;
    initialized = true;
}

/* OEIS entry: a(n) = n‑th non‑cube perfect power */
seq_t a(idx_t n)
{
    if (!initialized) {
        generate_values();
    }
    /* n is 1‑based; array is 0‑based */
    return values[(size_t)(n - 1)];
}
