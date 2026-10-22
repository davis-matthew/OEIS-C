/*
 * A350870  Numbers k = x.y such that x.y = (x+y)^2, when x and y have the
 *          same number of digits, "." means concatenation, and y may not
 *          begin with 0.
 * https://oeis.org/A350870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A350870  */
/*  a(n) = (A045913(n+1))^2  (Kaprekar numbers with equal parts) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/pow10.h"
#include "../utils/digit_count.h"

/* Helper: test whether a uint64_t is a Kaprekar number
 * (for the same‑length split used in A350870). */
static bool is_kaprekar(uint64_t n)
{
    if (n == 0) return false;

    /* number of digits of n */
    uint64_t k = digit_count(n);
    /* divisor 10^k as seq_t */
    seq_t divk = pow10((idx_t)k);
    seq_t divk_1 = pow10((idx_t)(k == 1 ? 0 : k - 1));

    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 in seq_t */
    seq_t y  = sq % divk;                    /* right part */
    seq_t x  = sq / divk;                    /* left part */

    /* y must have exactly k digits => y ≥ 10^(k-1) and y < 10^k  */
    if (y < divk_1 || y >= divk) return false;
    /* x must have exactly k digits => x ≥ 10^(k-1) and x < 10^k  */
    if (x < divk_1 || x >= divk) return false;

    /* check the Kaprekar condition: x + y = n */
    return (x + y) == (seq_t)n;
}

/* Main sequence function */
extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* out‑of‑bounds guard */

    /* static cache of already discovered terms */
    static seq_t *cache = NULL;                /* array of squares of Kaprekar numbers */
    static size_t cache_len = 0;               /* number of terms stored */
    static size_t cache_cap = 0;               /* allocated capacity */
    static uint64_t next_candidate = 1;        /* next n to test for Kaprekar */

    /* generate until we have at least n terms */
    while (cache_len < n) {
        if (is_kaprekar(next_candidate)) {
            seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate;
            if (cache_len == cache_cap) {      /* grow cache if needed */
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;     /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
        ++next_candidate;
    }

    /* cached values are naturally in increasing order */
    return cache[n - 1];
}
