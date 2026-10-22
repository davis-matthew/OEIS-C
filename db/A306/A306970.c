/*
 * A306970  Numbers of the form 8*k+3 not represented by
 *          2*x^2+4*y^2+4*y*z+9*z^2.
 * https://oeis.org/A306970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A306970
   Numbers of the form 8*k+3 not represented by 2*x^2+4*y^2+4*y*z+9*z^2.
   The code builds the set of representable numbers up to a growing
   limit and collects the gaps of the form 8*k+3.  It memoises the
   missing numbers so that calls for any n are fast.  All arithmetic
   is performed with integer types only. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of an unsigned 64‑bit integer */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    idx_t bit = (idx_t)1 << 62;          /* highest power of four <= 2^64 */
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Global memoisation state */
static seq_t *missing = NULL;          /* list of missing numbers */
static size_t missing_len = 0;
static size_t missing_cap = 0;
static idx_t   computed_limit = 0;     /* highest limit already processed */

/* Append a value to the missing list, growing it as needed. */
static void push_missing(idx_t val)
{
    if (missing_len == missing_cap) {
        missing_cap = missing_cap == 0 ? 16 : missing_cap << 1;
        missing = (seq_t *)realloc(missing, missing_cap * sizeof(seq_t));
    }
    missing[missing_len++] = (seq_t)val;
}

/* Count how many numbers of the form 8*k+3 are NOT representable
   by 2*x^2 + 4*y^2 + 4*y*z + 9*z^2 up to a given limit. */
static size_t count_missing_up_to(idx_t limit)
{
    if (limit == 0) return 0;

    uint8_t *rep = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!rep) return 0;          /* out of memory – unlikely for small limits */

    idx_t x_max = isqrt_u64(limit / 2);
    idx_t z_max = isqrt_u64(limit / 8);

    for (idx_t x = 0; x <= x_max; ++x) {
        idx_t base_x = 2ULL * x * x;          /* 2*x^2  */
        for (idx_t z = 0; z <= z_max; ++z) {
            idx_t base = base_x + 8ULL * z * z;      /* 2*x^2 + 8*z^2 */
            if (base > limit) break;

            idx_t rem = limit - base;
            idx_t w_max = isqrt_u64(rem);
            idx_t w_start = (z & 1) ? 1 : 0;   /* u ≡ z (mod 2) */
            for (idx_t w = w_start; w <= w_max; w += 2) {
                idx_t val = base + w * w;      /* complete value */
                rep[(size_t)val] = 1;
            }
        }
    }

    size_t cnt = 0;
    for (idx_t v = 3; v <= limit; v += 8) {
        if (!rep[(size_t)v]) ++cnt;
    }

    free(rep);
    return cnt;
}

/* Build the missing list up to a given limit and store it globally. */
static void build_missing_up_to(idx_t limit)
{
    /* Release any previous list. */
    free(missing);
    missing = NULL;
    missing_len = 0;
    missing_cap = 0;

    uint8_t *rep = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!rep) return;          /* out of memory – unlikely */

    idx_t x_max = isqrt_u64(limit / 2);
    idx_t z_max = isqrt_u64(limit / 8);

    for (idx_t x = 0; x <= x_max; ++x) {
        idx_t base_x = 2ULL * x * x;
        for (idx_t z = 0; z <= z_max; ++z) {
            idx_t base = base_x + 8ULL * z * z;
            if (base > limit) break;

            idx_t rem = limit - base;
            idx_t w_max = isqrt_u64(rem);
            idx_t w_start = (z & 1) ? 1 : 0;
            for (idx_t w = w_start; w <= w_max; w += 2) {
                idx_t val = base + w * w;
                rep[(size_t)val] = 1;
            }
        }
    }

    for (idx_t v = 3; v <= limit; v += 8) {
        if (!rep[(size_t)v]) push_missing(v);
    }

    free(rep);
    computed_limit = limit;
}

/* Public function implementing OEIS A306970. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, safe fallback */

    /* If we already have enough missing numbers, return directly. */
    if (missing_len >= (size_t)n)
        return missing[n - 1];

    /* Find a limit that guarantees at least n missing numbers. */
    idx_t limit = computed_limit > 0 ? computed_limit : 8;
    while (true) {
        size_t cnt = count_missing_up_to(limit);
        if (cnt >= n) break;
        /* Double the limit until sufficient. */
        if (limit > (UINT64_MAX >> 1)) {
            limit = UINT64_MAX;
            break;
        }
        limit <<= 1;
    }

    /* Build the missing list up to that limit. */
    build_missing_up_to(limit);

    /* Now we can answer the query. */
    return missing[n - 1];
}
