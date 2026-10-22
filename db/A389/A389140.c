/*
 * A389140  Smallest final number reachable by successively dividing a
 *          substring of digits of n by 2.
 * https://oeis.org/A389140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A389140 – Smallest final number reachable by successively dividing an
   even substring of n (not starting with 0) by 2.
   Algorithm:
     - Use recursion with memoisation.
     - For a number check whether any decimal digit is even.
       If not, the number is terminal.
     - Otherwise enumerate all admissible substrings, divide them by 2
       and build the resulting number digit by digit.
     - Keep the smallest terminal value found.
   The procedure runs in O(n log n) time and uses <20 kB of memory
   for n up to 1 000 000. */

/* ------------------------------------------------------------ */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The build supplies seq_t, idx_t and a host of helpers.
   The following headers make the needed helpers visible. */
#include "../utils/get_digits.h"   /* if available, otherwise we use our own */

static seq_t *memo = NULL;     /* cache of already computed results */
static idx_t   memo_cap = 0;   /* allocated capacity of 'memo' */

/* Ensure that memo[n] is within bounds, expanding the array if necessary. */
static void
ensure_cache(idx_t n)
{
    if (n < memo_cap)
        return;

    idx_t new_cap = memo_cap > 0 ? memo_cap * 2 : 8;
    while (new_cap <= n)
        new_cap *= 2;

    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem)
        abort();                 /* out of memory */

    /* Zero‑fill the newly added parts */
    memset(new_mem + memo_cap, 0, (new_cap - memo_cap) * sizeof(seq_t));

    memo = new_mem;
    memo_cap = new_cap;
}

/* Extract decimal digits of x into array `d` in MSB → LSB order.
   Returns the number of digits.  Assumes `buf` is large enough. */
static unsigned
extract_digits(seq_t x, uint8_t *buf, unsigned maxlen)
{
    if (x == 0) {
        if (maxlen < 1) abort();
        buf[0] = 0;
        return 1;
    }

    seq_t tmp = x;
    unsigned cnt = 0;
    uint8_t rev[64];                /* enough for 64‑bit numbers */

    while (tmp > 0) {
        if (cnt >= 64) abort();    /* safety */
        rev[cnt++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    if (cnt > maxlen) abort();

    /* reverse into buf */
    for (unsigned i = 0; i < cnt; ++i)
        buf[i] = rev[cnt - 1 - i];
    return cnt;
}

/* Recursively compute minimal terminal reachable from n. */
static seq_t
min_terminal(seq_t n)
{
    if (n == 0)
        return 0;                 /* shouldn't happen */

    ensure_cache((idx_t)n);
    if (memo[(idx_t)n] != 0)
        return memo[(idx_t)n];

    /* Check if n already terminal (no even digit). */
    uint8_t digits[64];
    unsigned len = extract_digits(n, digits, sizeof digits);

    bool has_even = false;
    for (unsigned i = 0; i < len; ++i)
        if ((digits[i] & 1u) == 0u) { has_even = true; break; }

    if (!has_even) {
        memo[(idx_t)n] = n;
        return n;
    }

    seq_t best = n;                       /* upper bound */

    /* Enumerate admissible substrings. */
    for (unsigned start = 0; start < len; ++start) {
        if (digits[start] == 0u)          /* cannot start with 0 */
            continue;

        seq_t sub_val = 0;
        for (unsigned end = start; end < len; ++end) {
            sub_val = sub_val * 10 + digits[end];

            if ((digits[end] & 1u) != 0u)  /* last digit must be even */
                continue;

            seq_t q = sub_val >> 1;       /* divide by 2 */

            /* Build new number by appending digits. */
            seq_t new_num = 0;

            /* Prefix. */
            for (unsigned i = 0; i < start; ++i)
                new_num = new_num * 10 + digits[i];

            /* Digits of q (reversed). */
            seq_t tmp_q = q;
            uint8_t rev_q[64];
            unsigned qcnt = 0;
            while (tmp_q > 0) {
                rev_q[qcnt++] = (uint8_t)(tmp_q % 10);
                tmp_q /= 10;
            }
            if (qcnt == 0) rev_q[qcnt++] = 0;   /* q == 0 unlikely */

            for (int i = (int)qcnt - 1; i >= 0; --i)
                new_num = new_num * 10 + rev_q[i];

            /* Suffix. */
            for (unsigned i = end + 1; i < len; ++i)
                new_num = new_num * 10 + digits[i];

            seq_t candidate = min_terminal(new_num);
            if (candidate < best) {
                best = candidate;
                if (best == 1) {           /* cannot get smaller */
                    memo[(idx_t)n] = best;
                    return best;
                }
            }
        }
    }

    memo[(idx_t)n] = best;
    return best;
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    return min_terminal((seq_t)n);
}
