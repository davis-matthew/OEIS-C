/*
 * A368198  a(n) gives the number of ways to go from n to 1 with steps
 *          consisting of replacing a positive number without leading zero,
 *          say m, appearing in the decimal expansion of a number, by a proper
 *          divisor of m.
 * https://oeis.org/A368198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 187, u32 n <= 1367, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9307 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A368198 – number of ways to go from n to 1 by replacing any
   contiguous decimal block m of the current number by a proper divisor
   of m.  The computation uses memoization and deduplicates equal
   predecessors to avoid counting the same transition more than once.
   Recurrence:  a(1)=1.  For n>1, a(n)=∑_{k}a(k), where k runs over all
   distinct integers obtained from n by one such replacement. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* convert an integer into its decimal string in buf (no leading zeros).
   len is set to the number of digits written. */
static inline void int_to_digits(idx_t val, char *buf, idx_t *len)
{
    idx_t i = 0;
    do {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    } while (val);
    /* reverse */
    for (idx_t a = 0, b = i - 1; a < b; ++a, --b) {
        char t = buf[a];
        buf[a] = buf[b];
        buf[b] = t;
    }
    *len = i;
}

/* Global memoization storage for a(n) */
static seq_t *memo          = NULL;   /* array indexed by 1..max */
static idx_t memo_capacity  = 0;      /* allocated size (>=max+1) */
static idx_t max_computed   = 0;      /* greatest n already computed */

/* Boolean array used to deduplicate successors of an integer. */
static uint8_t *visited      = NULL;   /* array indexed by 1..max-1 */
static idx_t visited_capacity = 0;

/* Public function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* out of range – not used */

    /* Grow memo buffer if necessary */
    if (n + 1 > memo_capacity) {
        size_t new_cap = (size_t)(n + 1);
        ensure_capacity(new_cap, &memo_capacity, OEISC_VPP(&memo), sizeof(seq_t));
    }

    /* Grow visited buffer if necessary */
    if (n + 1 > visited_capacity) {
        size_t new_cap = (size_t)(n + 1);
        ensure_capacity(new_cap, &visited_capacity, OEISC_VPP(&visited),
                         sizeof(uint8_t));
    }

    /* Compute values up to n, filling in memo array */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            memo[1] = 1;
            continue;
        }

        /* reset visited marks for this i */
        memset(visited, 0, i);      /* indices 1..i-1 used */

        /* write i to digit array */
        char digits[20];
        idx_t len = 0;
        int_to_digits(i, digits, &len);

        seq_t total = 0;

        /* iterate over all substrings that start with a non‑zero digit */
        for (idx_t start = 0; start < len; ++start) {
            if (digits[start] == '0') continue;      /* leading zero not allowed */
            idx_t subval = 0;
            for (idx_t end = start; end < len; ++end) {
                subval = subval * 10 + (digits[end] - '0');

                /* enumerate proper divisors of subval */
                for (idx_t d = 1; d * d <= subval; ++d) {
                    if (subval % d != 0) continue;

                    /* first divisor d */
                    if (d < subval) {
                        /* build new number by replacing digits[start..end] with d */
                        char new_buf[20];
                        idx_t p = 0;

                        /* prefix */
                        for (idx_t k = 0; k < start; ++k)
                            new_buf[p++] = digits[k];

                        /* divisor digits */
                        char dbuf[20];
                        idx_t dlen = 0;
                        int_to_digits(d, dbuf, &dlen);
                        for (idx_t k = 0; k < dlen; ++k)
                            new_buf[p++] = dbuf[k];

                        /* suffix */
                        for (idx_t k = end + 1; k < len; ++k)
                            new_buf[p++] = digits[k];

                        /* convert to integer */
                        idx_t val = 0;
                        for (idx_t k = 0; k < p; ++k)
                            val = val * 10 + (new_buf[k] - '0');

                        /* deduplicate */
                        if (!visited[val]) {
                            visited[val] = 1;
                            total += memo[val];
                        }
                    }

                    /* second divisor subval/d if distinct */
                    idx_t d2 = subval / d;
                    if (d2 != d && d2 < subval) {
                        char new_buf[20];
                        idx_t p = 0;
                        for (idx_t k = 0; k < start; ++k)
                            new_buf[p++] = digits[k];
                        char dbuf[20];
                        idx_t dlen = 0;
                        int_to_digits(d2, dbuf, &dlen);
                        for (idx_t k = 0; k < dlen; ++k)
                            new_buf[p++] = dbuf[k];
                        for (idx_t k = end + 1; k < len; ++k)
                            new_buf[p++] = digits[k];
                        idx_t val = 0;
                        for (idx_t k = 0; k < p; ++k)
                            val = val * 10 + (new_buf[k] - '0');
                        if (!visited[val]) {
                            visited[val] = 1;
                            total += memo[val];
                        }
                    }
                }
            }
        }

        memo[i] = total;
    }

    max_computed = n;
    return memo[n];
}
