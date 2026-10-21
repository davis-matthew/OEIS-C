/*
 * A299985  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 5, and no term occurs twice.
 * https://oeis.org/A299985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction of A299985:  a(n) is the smallest unused
   non‑negative integer such that a(n)+a(n+1) contains the digit 5,
   and the sequence is a permutation of the non‑negative integers. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Static state ------------------------------------------------------ */
static seq_t *seq_cache = NULL;   /* memoised sequence terms          */
static size_t cache_cap = 0;      /* allocated capacity of seq_cache  */
static size_t cache_len = 0;      /* number of terms already computed */

static unsigned char *used = NULL;/* bitset of numbers already used   */
static size_t used_cap = 0;       /* capacity of used                 */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Helper: Ensure cache can store at least one more element           */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Helper: Ensure used bitset can index at least m                     */
static void ensure_used_capacity(idx_t m)
{
    size_t need = (size_t)m + 1;
    if (used_cap >= need) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    unsigned char *new_used = (unsigned char *)realloc(used, new_cap);
    /* Zero the newly allocated region */
    memset(new_used + used_cap, 0, new_cap - used_cap);
    used = new_used;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Test whether decimal representation of x contains digit 5           */
static bool has_digit5(seq_t x)
{
    if (x == (seq_t)0) return false;
    while (x != (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)5) return true;
        x /= (seq_t)10;
    }
    return false;
}

/* Test whether a+b contains a digit 5                              */
static bool digit5_in_sum(seq_t a, seq_t b)
{
    return has_digit5(a + b);
}

/* ------------------------------------------------------------------ */
/* Public entry ******************************************************/
seq_t a(idx_t n)
{
    /* Initialise on first invocation */
    if (cache_len == 0) {
        ensure_cache_capacity();
        seq_cache[cache_len++] = (seq_t)0;          /* a(0) = 0   */
        ensure_used_capacity(0);
        used[0] = 1;
    }

    /* Generate terms until we have a(0 … n) */
    while (cache_len <= (size_t)n) {
        seq_t prev = seq_cache[cache_len - 1];
        idx_t m = 0;

        /* Search for the smallest unused integer m such that
           prev + m has digit 5. */
        for (;;) {
            ensure_used_capacity(m);
            if (used[(size_t)m]) { m++; continue; }

            if (digit5_in_sum(prev, (seq_t)m)) {
                /* Accept this number as the next term */
                ensure_cache_capacity();
                seq_cache[cache_len++] = (seq_t)m;
                used[(size_t)m] = 1;
                break;
            }
            m++;
        }
    }

    return seq_cache[(size_t)n];
}
