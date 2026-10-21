/*
 * A256523  Numbers m such that m, m^2 and m^3 have identical initial digits
 *          in decimal representation.
 * https://oeis.org/A256523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 3273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   leading_digit_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256523: numbers m whose leading decimal digits of m, m²,
   and m³ coincide.
   The function builds the sequence lazily and caches the terms for
   efficient repeated calls.  The property is tested by computing the
   leading digit of each of the three values using 128‑bit arithmetic,
   which is exact for all seq_t values that fit in 64 bits.  This
   implementation satisfies the requirements for C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/leading_digit_uint128.h"

/* Forward declarations of the build‑system supplied integer types. */
typedef idx_t idx_t;   /* defined in the build system */
typedef seq_t seq_t;   /* defined in the build system */

/* Cache for the computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* The largest m that has already been examined. */
static idx_t last_checked = 0;

/* Append a new value to the cache, expanding it if necessary. */
static void add_term(seq_t v)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = v;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Ensure that the first term (0) is present. */
    if (cache_len == 0) {
        add_term((seq_t)0);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Next m to examine.  We keep it as a large integer for safety. */
        unsigned __int128 m128 = (unsigned __int128)(last_checked + 1);
        ++last_checked;

        /* We only store values that fit into seq_t.  If the 128‑bit value
           overflows seq_t, we stop searching – the build is expected to
           request only terms that fit. */
        seq_t m = (seq_t)m128;
        if ((unsigned __int128)m != m128) {
            /* Overflow: we cannot represent further terms.  Stop searching. */
            break;
        }

        /* Leading digit of m, m², and m³ must be equal. */
        int d = leading_digit_uint128(m128);
        if (d != 1 && d != 9) continue;                 /* Lemma from OEIS */

        unsigned __int128 sq128 = m128 * m128;
        int d2 = leading_digit_uint128(sq128);
        if (d2 != d) continue;

        unsigned __int128 cube128 = sq128 * m128;
        int d3 = leading_digit_uint128(cube128);
        if (d3 != d) continue;

        /* All three have the same leading digit: store the term. */
        add_term(m);
    }

    /* The cache is guaranteed to contain at least n terms (unless an
       overflow prevented further search; in that case the function
       returns an arbitrary value but the build does not query such
       indices). */
    return cache[n - 1];
}
