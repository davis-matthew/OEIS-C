/*
 * A345790  Numbers that are the sum of eight cubes in exactly eight ways.
 * https://oeis.org/A345790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 176, u32 n <= 176, u64 n <= 176, u128 n <= 176,
 *            big any n
 * Helpers:   alloc_uint8_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A345790: numbers that are the sum of eight cubes in exactly
   eight distinct unsigned integer ways.  The implementation builds an
   incremental DP table of counts for sums up to a growing bound.  The
   resulting terms are cached, so any call to a(n) with n ≥ 1 returns
   the correct nth term regardless of call order. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/alloc_uint8_array.h"

/* ---------- Static state ---------- */
static seq_t *terms      = NULL;  /* array of terms found so far          */
static size_t terms_len  = 0;     /* number of elements in terms          */
static size_t terms_cap  = 0;     /* capacity of terms array              */
static idx_t   maxM       = 0;    /* maximum sum value processed already   */

/* ---------- Helper: grow the terms array ---------- */
static void ensure_terms_capacity(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    if (terms_cap == 0) terms_cap = 1024;
    while (terms_cap < new_cap) terms_cap <<= 1;
    /* realloc may be used directly – the build ensures seq_t is trivially
       constructible so a plain cast is sufficient even in C++. */
    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
}

/* ---------- Helper: compute terms up to a required index ---------- */
static void ensure_terms_up_to(idx_t nreq)
{
    if (terms_len >= (size_t)nreq) return;     /* already enough terms */

    /* Grow the maximum bound until we have nreq terms. */
    idx_t newM = (maxM == 0) ? 2000 : maxM;
    while (terms_len < (size_t)nreq) {
        newM <<= 1;          /* double the bound each pass */
        size_t width  = (size_t)(newM + 1);      /* number of sums per row */
        size_t dp_size = width * 9;              /* 9 rows: 0..8 cubes */

        uint8_t *dp = alloc_uint8_array(dp_size);
        if (!dp) return;      /* memory exhaustion – give up */

        dp[0] = 1;           /* dp[0][0] = 1 */

        for (idx_t k = 1; ; ++k) {
            idx_t cube = k * k * k;
            if (cube > newM) break;

            for (idx_t i = 1; i <= 8; ++i) {
                size_t off_prev = (size_t)(i - 1) * width;
                size_t off_cur  = (size_t) i * width;
                for (size_t s = (size_t)cube; s < width; ++s) {
                    uint8_t prev_val = dp[off_prev + s - cube];
                    if (!prev_val) continue;
                    uint8_t new_val = dp[off_cur + s] + prev_val;
                    dp[off_cur + s] = (new_val > 8) ? 9 : new_val; /* clamp */
                }
            }
        }

        /* Append all new numbers (> oldM) that have exactly 8 representations. */
        size_t start = (size_t)maxM + 1;
        size_t end   = (size_t)newM;
        size_t row8 = 8 * width;
        for (size_t s = start; s <= end; ++s) {
            if (dp[row8 + s] == 8) {
                ensure_terms_capacity(terms_len + 1);
                terms[terms_len++] = (seq_t)s;
            }
        }

        free(dp);
        maxM = newM;
    }
}

/* ---------- Public API ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n==0 */
    ensure_terms_up_to(n);                  /* guarantee we have n terms */
    if ((size_t)n <= terms_len) {
        return terms[n - 1];
    }
    /* In unexpected situations (e.g., memory exhaustion) we return 0. */
    return 0;
}
