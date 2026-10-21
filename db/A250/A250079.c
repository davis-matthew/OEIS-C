/*
 * A250079  Number of length n+5 0..6 arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 25, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250079: a(n) = number of length n+5 arrays over 0..6 such that
   every six consecutive terms have the property that the maximum of
   some two terms equals the minimum of the remaining four terms. */

/* The implementation uses dynamic programming over the last 5 digits.
   States are encoded as a base‑7 number of length 5 (16807 states).
   From a state and a new digit we check whether the new window of 6
   satisfies the condition.  The condition holds iff, after sorting the
   six values, the second and third smallest are equal.  This is
   equivalent to: there exists a value v that occurs at least twice and
   all other elements are >= v, with at most one element < v. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATE 16807          /* 7^5 possible 5‑digit suffixes */
#define BASE      7
#define STATE4    2401            /* 7^4 */

static uint8_t  *valid_tbl = NULL;   /* valid_tbl[state*7 + digit] == 1 if window OK */
static uint16_t *next_tbl  = NULL;   /* next_tbl[state*7 + digit] = new state */

/* DP buffers */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;

/* Cached values of a(n) */
static seq_t *a_cache   = NULL;
static idx_t  cache_len = 0;   /* current capacity of a_cache */

/* Initialise valid_tbl and next_tbl once */
static void init_tables(void)
{
    if (valid_tbl) return;   /* already initialised */

    valid_tbl = (uint8_t *)malloc(MAX_STATE * BASE * sizeof(uint8_t));
    next_tbl  = (uint16_t *)malloc(MAX_STATE * BASE * sizeof(uint16_t));
    if (!valid_tbl || !next_tbl) exit(EXIT_FAILURE);

    /* temporary buffer for sorting 6 digits */
    int w[6];

    for (uint32_t s = 0; s < MAX_STATE; ++s) {
        /* decompose state into 5 digits (a,b,c,d,e) */
        int a = s / STATE4;
        int rem = s % STATE4;
        int b = rem / 343;        /* 7^3 = 343 */
        rem %= 343;
        int c = rem / 49;         /* 7^2 = 49 */
        rem %= 49;
        int d = rem / 7;          /* 7^1 */
        int e = rem % 7;          /* 7^0 */

        for (int digit = 0; digit < BASE; ++digit) {
            /* form 6‐tuple */
            w[0] = a; w[1] = b; w[2] = c;
            w[3] = d; w[4] = e; w[5] = digit;

            /* simple insertion sort */
            for (int i = 1; i < 6; ++i) {
                int key = w[i];
                int j = i - 1;
                while (j >= 0 && w[j] > key) { w[j + 1] = w[j]; --j; }
                w[j + 1] = key;
            }

            /* condition: second and third smallest are equal */
            bool ok = (w[1] == w[2]);

            uint32_t idx = s * BASE + digit;
            if (ok) {
                valid_tbl[idx] = 1;
                /* new state: drop the oldest digit (a) and append digit */
                next_tbl[idx] = (uint16_t)((s % STATE4) * BASE + digit);
            } else {
                valid_tbl[idx] = 0;
                next_tbl[idx] = 0;
            }
        }
    }
}

/* Ensure a_cache can store at least n */
static void ensure_cache(idx_t n)
{
    if (n + 1 < cache_len) return;
    idx_t new_len = (n + 8) * 2;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    a_cache = new_cache;
    cache_len = new_len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index 0 is unused */
    init_tables();

    /* Initialise DP buffers on first call */
    if (!dp_cur) {
        dp_cur  = (seq_t *)calloc(MAX_STATE, sizeof(seq_t));
        dp_next = (seq_t *)calloc(MAX_STATE, sizeof(seq_t));
        if (!dp_cur || !dp_next) exit(EXIT_FAILURE);
        for (uint32_t i = 0; i < MAX_STATE; ++i) dp_cur[i] = (seq_t)1;
    }

    /* Initialise cache on first call */
    if (!a_cache) {
        a_cache = (seq_t *)calloc(16, sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        a_cache[0] = 0;  /* unused sentinel */
    }

    static idx_t current_length = 5; /* length represented by dp_cur */
    static idx_t cached_n = 0;       /* largest n for which a_cache[n] is set */

    ensure_cache(n);

    while (cached_n < n) {
        idx_t next_len = current_length + 1;   /* new sequence length */
        idx_t k = next_len - 5;                /* index for a(k) */

        memset(dp_next, 0, MAX_STATE * sizeof(seq_t));

        for (uint32_t s = 0; s < MAX_STATE; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            for (int d = 0; d < BASE; ++d) {
                uint32_t idx = s * BASE + d;
                if (!valid_tbl[idx]) continue;
                uint16_t ns = next_tbl[idx];
                dp_next[ns] += val;
            }
        }

        /* Sum all states to get a(k) */
        seq_t total = (seq_t)0;
        for (uint32_t i = 0; i < MAX_STATE; ++i) total += dp_next[i];
        a_cache[k] = total;
        cached_n = k;

        /* Prepare for next iteration */
        current_length = next_len;
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return a_cache[n];
}
