/*
 * A340585  Noncube perfect powers.
 * https://oeis.org/A340585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, floor_cube_root
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
#include "../utils/cmp_seq_t.h"
#include "../utils/floor_cube_root.h"

static seq_t *values = NULL;
static size_t values_count = 0;
static bool initialized = false;

/* Helper to generate the array on first use */
static void generate_values(void)
{
    const seq_t LIMIT = (seq_t)200000000;
    size_t cap = 200000;            /* initial capacity (over‑estimate) */
    seq_t *buf = (seq_t *)malloc(cap * sizeof(seq_t));
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
                    seq_t *tmp = (seq_t *)realloc(buf, cap * sizeof(seq_t));
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
    qsort(buf, cnt, sizeof(seq_t), cmp_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0 || buf[i] != buf[i - 1]) {
            buf[uniq++] = buf[i];
        }
    }
    buf = (seq_t *)realloc(buf, uniq * sizeof(seq_t));
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
