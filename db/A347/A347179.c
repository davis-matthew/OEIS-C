/*
 * A347179  a(1) = 1; for n > 1, a(n) = smallest distinct positive integer
 *          such that gcd(a(n),a(n-k)) = 1, where k is each divisor of a(n)
 *          and n - k >= 1.
 * https://oeis.org/A347179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation computes OEIS sequence A347179.
 *
 * Definition:
 *   a(1) = 1
 *   For n > 1, a(n) is the smallest positive integer that
 *   (i) has not appeared earlier in the sequence
 *   (ii) for every divisor d of a(n) with d ≤ n-1,
 *        gcd(a(n), a(n-d)) = 1
 *
 * The code stores the sequence as it is needed, allowing random
 * calls to a(idx_t n).  All arithmetic is performed with the
 * integer type seq_t, while indices use idx_t.  Helper functions
 * from the provided utilities are used where appropriate.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"

/* data structures for memoisation */
static seq_t *seq_arr = NULL;   /* 0‑based: seq_arr[i] = a(i+1) */
static idx_t  seq_len = 0;      /* number of computed terms (>=0) */
static idx_t  seq_cap = 0;      /* allocated capacity */

static bool *used = NULL;       /* used[x] == true ⇔ x already appeared */
static size_t used_cap = 0;     /* allocated capacity of use array */

/* enlarge seq_arr to accommodate at least 'need' terms */
static void ensure_seq_capacity(idx_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap : 256;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* enlarge used[] to accommodate index 'idx' (0‑based) */
static void ensure_used_capacity(size_t idx)
{
    if (used_cap > idx) return;
    size_t new_cap = used_cap ? used_cap : 256;
    while (new_cap <= idx) new_cap <<= 1;
    used = (bool *)realloc(used, new_cap);
    /* newly allocated bytes are undefined; set them to 0 */
    for (size_t i = used_cap; i < new_cap; ++i) used[i] = false;
    used_cap = new_cap;
}

/* test whether candidate m can be a(n) */
static bool candidate_ok(idx_t n, seq_t m)
{
    /* omit already used values (checked by caller) */
    unsigned long long m64 = (unsigned long long)m;
    for (unsigned long long d = 1; d * d <= m64; ++d) {
        if (m64 % d) continue;
        /* divisor d */
        if (d <= (unsigned long long)(n - 1)) {
            idx_t   pos = n - (idx_t)d;        /* 1‑based position */
            seq_t   prev = seq_arr[pos - 1];   /* a(n-d) */
            if (gcd(m, prev) != 1) return false;
        }
        /* divisor d2 = m/d */
        unsigned long long d2 = m64 / d;
        if (d2 == d) continue;
        if (d2 <= (unsigned long long)(n - 1)) {
            idx_t   pos = n - (idx_t)d2;
            seq_t   prev = seq_arr[pos - 1];
            if (gcd(m, prev) != 1) return false;
        }
    }
    return true;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n < 1 */

    /* compute sequence up to n */
    if (seq_len < n) {
        /* allocate/extend arrays as needed */
        ensure_seq_capacity(n);
        ensure_used_capacity((size_t)(n * 2)); /* a rough upper bound */

        for (idx_t i = seq_len + 1; i <= n; ++i) {  /* build a(i) */
            seq_t cand = 1;
            for (;;) {
                /* skip already used numbers */
                size_t cand_idx = (size_t)cand;
                if (cand_idx >= used_cap) ensure_used_capacity(cand_idx);
                if (used[cand_idx]) { ++cand; continue; }

                /* apply constraints */
                if (candidate_ok(i, cand)) {
                    seq_arr[i - 1] = cand;
                    used[cand_idx] = true;
                    break;
                }
                ++cand;
            }
        }
        seq_len = n;
    }
    return seq_arr[n - 1];
}
