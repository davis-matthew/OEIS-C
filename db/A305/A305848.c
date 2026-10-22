/*
 * A305848  Solution b() of the complementary equation a(n) + b(n) = 5n, where
 *          a(1) = 1. See Comments.
 * https://oeis.org/A305848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A305848 – sequence of b(n) where a(n)+b(n)=5n, a(1)=1
 *
 *  b(n) is computed by building the complementary increasing sequences
 *  a(n) and b(n) greedily.  a(n) is the smallest unused integer > a(n-1)
 *  such that b(n)=5n-a(n) is unused and > b(n-1).  The function returns
 *  exactly the requested b(n).  Memoisation is used so that calls can
 *  arrive in any order and each term after the first true computation is
 *  produced in O(1).
 *
 *  Since no floating point is allowed, all arithmetic is performed in the
 *  integer type seq_t, which may be fixed width or an arbitrary‑precision
 *  class.  The static tables a_vals and used are resized with realloc
 *  (explicit casts are required for C++).  No external utilities are
 *  required.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for the a() sequence (1‑based index; a_vals[0] unused) */
static seq_t *a_vals = NULL;      /* a_vals[i] = a(i) */
static size_t a_len = 0;          /* largest n for which a_vals[n] is valid */

/* bitmask marking numbers already used by either a() or b() */
static unsigned char *used = NULL;/* used[v] == 1 iff v is already taken */
static size_t used_cap = 0;       /* current capacity of 'used' array */

/* Expand the cache so that a_vals[1..n] are known and all values up to
 * 5*n are marked in 'used'.  The function is idempotent when called with
 * n <= a_len. */
static void ensure_cache(idx_t n)
{
    if (n <= a_len)
        return;                      /* already cached */

    /* Extend the a_vals array. */
    size_t new_len = (size_t)n + 1; /* include index 0 */
    a_vals = (seq_t *)realloc(a_vals, new_len * sizeof(seq_t));

    /* Ensure 'used' covers values up to 5*n. */
    size_t needed_used = 5 * (size_t)n + 1;
    if (needed_used > used_cap) {
        used = (unsigned char *)realloc(used,
                                        needed_used * sizeof(unsigned char));
        /* clear new entries */
        memset(used + used_cap, 0,
               (needed_used - used_cap) * sizeof(unsigned char));
        used_cap = needed_used;
    }

    /* If we have just started, initialise the first term. */
    if (a_len == 0) {
        a_vals[1] = (seq_t)1;          /* a(1)=1 */
        used[1] = 1;                   /* mark a(1) as used */
        used[(size_t)(5 - 1)] = 1;     /* b(1)=4 marked as used */
        a_len = 1;
    }

    /* Variables holding the previous term. */
    seq_t a_prev = a_vals[a_len];
    seq_t five   = (seq_t)5;
    seq_t b_prev = five * (seq_t)a_len - a_prev;  /* b(a_len) */

    /* Build terms from a_len+1 up to n. */
    for (idx_t i = a_len + 1; i <= n; ++i) {
        /* Start candidate just above the previous a. */
        seq_t candidate = a_prev + (seq_t)1;
        seq_t next_b;
        /* Find a valid candidate. */
        while (1) {
            next_b = five * (seq_t)i - candidate;    /* b(i) */
            if (next_b <= b_prev) {                  /* not increasing */
                ++candidate;
                continue;
            }
            /* Check if candidate or next_b are already used. */
            size_t c_idx = (size_t)candidate;
            size_t b_idx = (size_t)next_b;
            if (c_idx < used_cap && used[c_idx]) {
                ++candidate;
                continue;
            }
            if (b_idx < used_cap && used[b_idx]) {
                ++candidate;
                continue;
            }
            break; /* valid a(i) found */
        }

        /* Store the found values and mark them as used. */
        a_vals[i] = candidate;
        used[(size_t)candidate] = 1;
        used[(size_t)(five * (seq_t)i - candidate)] = 1;

        /* Update previous values for next iteration. */
        a_prev = candidate;
        b_prev = five * (seq_t)i - candidate;
    }

    a_len = n;
}

/*  Public function required by the build system.  Returns b(n).  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    seq_t a_val = a_vals[n];
    seq_t five = (seq_t)5;
    return five * (seq_t)n - a_val;
}
