/*
 * A394413  a(1) = 1, a(2) = 2; for n > 2, a(n) is an unused positive number
 *          that shares a factor with a(n-1) while being as close as possible
 *          to a(n-2). If two such numbers exist then the smaller is chosen.
 * https://oeis.org/A394413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A394413: a(1)=1, a(2)=2; for n>2, a(n) is the smallest unused positive
   number sharing a factor with a(n−1) that is as close as possible to a(n−2). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *seq_buf = NULL;   /* stored sequence values */
static size_t seq_cnt = 0;      /* number of values stored */
static size_t seq_cap = 0;      /* capacity of seq_buf */

static bool *used = NULL;       /* bit‑set of used values (size_t index) */
static size_t used_cap = 0;     /* capacity of used */

/* Ensure seq_buf holds at least `need` entries. */
static void seq_ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 4;
    while (new_cap < need)
        new_cap <<= 1;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used bit‑set can index value `v`. */
static void used_ensure_capacity(size_t v)
{
    if (v < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= v)
        new_cap <<= 1;
    used = (bool *)realloc(used, new_cap * sizeof(bool));
    for (size_t i = used_cap; i < new_cap; ++i)
        used[i] = false;
    used_cap = new_cap;
}

/* Mark a value as used. */
static void mark_used(seq_t v)
{
    size_t idx;
    if (v > (seq_t)SIZE_MAX)   /* too big for our bit‑set: ignore */
        return;
    idx = (size_t)v;
    used_ensure_capacity(idx);
    used[idx] = true;
}

/* Test whether a value has already been used. */
static bool is_used(seq_t v)
{
    size_t idx;
    if (v > (seq_t)SIZE_MAX)
        return false;
    idx = (size_t)v;
    if (idx >= used_cap)
        return false;
    return used[idx];
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n=0 */

    /* Initialize the first two terms lazily. */
    if (seq_cnt < 2) {
        seq_ensure_capacity(2);
        seq_buf[0] = (seq_t)1;
        seq_buf[1] = (seq_t)2;
        seq_cnt = 2;
        mark_used((seq_t)1);
        mark_used((seq_t)2);
    }

    /* Generate terms up to the requested index. */
    while (seq_cnt < n) {
        seq_t prev     = seq_buf[seq_cnt - 1];
        seq_t prevPrev = seq_buf[seq_cnt - 2];
        seq_t candidate = 0;

        /* Search outward from prevPrev. */
        seq_t d = 1;
        while (!candidate) {
            /* left side: prevPrev - d (must be >=1) */
            if (prevPrev > d) {
                seq_t left = prevPrev - d;
                if (!is_used(left) && gcd(prev, left) > (seq_t)1) {
                    candidate = left;
                    break;
                }
            }
            /* right side: prevPrev + d */
            seq_t right = prevPrev + d;
            if (!is_used(right) && gcd(prev, right) > (seq_t)1) {
                candidate = right;
                break;
            }
            ++d;
        }

        seq_ensure_capacity(seq_cnt + 1);
        seq_buf[seq_cnt++] = candidate;
        mark_used(candidate);
    }

    return seq_buf[n - 1];
}
