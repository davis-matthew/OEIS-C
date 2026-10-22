/*
 * A336890  Numbers that eventually reach the fixed point 8208 under "x -->
 *          sum of the fourth powers of digits of x".
 * https://oeis.org/A336890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336890 – numbers that eventually reach the fixed point 8208
   under the map x → sum of the fourth powers of the decimal digits
   of x.  The function returns the n-th smallest such number
   (offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 100000               /* values below LIMIT are memoised */

/* pre‑computed fourth powers of 0–9 as seq_t values */
static const seq_t pow4[10] = {
    (seq_t)0,   (seq_t)1,   (seq_t)16,  (seq_t)81,
    (seq_t)256, (seq_t)625, (seq_t)1296, (seq_t)2401,
    (seq_t)4096, (seq_t)6561
};

/* sum of the fourth powers of the decimal digits of v */
static inline seq_t sum_fourth_digits(seq_t v)
{
    seq_t s = (seq_t)0;
    while (v != (seq_t)0) {
        uint64_t d = (uint64_t)(v % (seq_t)10);
        s += pow4[d];
        v /= (seq_t)10;
    }
    return s;
}

/* memoisation table for small numbers:
   -1 = unknown, 0 = fails, 1 = succeeds   */
static signed char memo[LIMIT];
static bool memoised = false;
static void init_memo(void)
{
    if (memoised) return;
    for (size_t i = 0; i < LIMIT; ++i) memo[i] = -1;
    memoised = true;
}

/* determine whether x ultimately reaches 8208 */
static bool reaches_target(seq_t x)
{
    init_memo();
    if (x == (seq_t)8208) return true;   /* quick return */

    seq_t cur = x;
    seq_t path[64];
    size_t plen = 0;
    bool reached = false;

    while (true) {
        /* success or failure condition */
        if (cur == (seq_t)8208) { reached = true; break; }
        if (cur == (seq_t)0) { reached = false; break; }

        /* check memoised result for small numbers */
        if (cur < (seq_t)LIMIT) {
            signed char m = memo[(size_t)cur];
            if (m != -1) { reached = (m == 1); break; }
        }

        /* cycle detection in the current path */
        for (size_t i = 0; i < plen; ++i) {
            if (path[i] == cur) { reached = false; goto out; }
        }

        /* store current number if we have space */
        if (plen < sizeof(path) / sizeof(path[0])) path[plen++] = cur;

        /* move to next number in the iteration */
        cur = sum_fourth_digits(cur);
    }

out:
    /* memoise the outcome for all small numbers encountered */
    for (size_t i = 0; i < plen; ++i) {
        seq_t val = path[i];
        if (val < (seq_t)LIMIT) memo[(size_t)val] = reached ? 1 : 0;
    }
    return reached;
}

/* storage for found terms in ascending order */
static seq_t *found = NULL;
static size_t found_count = 0;
static size_t found_cap   = 0;
static seq_t last_checked = (seq_t)0;

/* ensure capacity for at least one more element */
static void ensure_found_capacity(void)
{
    if (found_count + 1 > found_cap) {
        size_t new_cap = (found_cap == 0) ? 16 : found_cap * 2;
        seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        found = tmp;
        found_cap = new_cap;
    }
}

/* main sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of bounds – not used */

    /* extend the sequence until we have at least n terms */
    while (found_count < n) {
        last_checked++;
        if (reaches_target(last_checked)) {
            ensure_found_capacity();
            found[found_count++] = last_checked;
        }
    }
    return found[n - 1];
}
