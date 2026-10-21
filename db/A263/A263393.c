/*
 * A263393  Smallest Fibonacci number containing Fibonacci(n) as a proper
 *          substring in decimal notation.
 * https://oeis.org/A263393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest Fibonacci number containing Fibonacci(n) as a
 *            proper substring in decimal notation
 * Range:     u8 none, u16 n <= 6, u32 n <= 10, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* we use the helper that converts a seq_t to a decimal string */
#include "../utils/seq_to_str.h"

/* ------------------------------------------------------------------ */
/*  cache for memoised answers: each index stores the computed value   */
/* ------------------------------------------------------------------ */
static seq_t   *ans_val   = NULL;
static bool    *ans_seen  = NULL;
static size_t   ans_cache_len = 0;

/* enlarge the cache so that entry 'idx' exists                      */
static void ensure_ans_cache(idx_t idx)
{
    if (ans_cache_len > idx) return;
    size_t new_len = (size_t)idx + 1;
    seq_t *new_val = (seq_t *)realloc(ans_val, new_len * sizeof(seq_t));
    if (!new_val) exit(1);
    bool *new_seen = (bool *)realloc(ans_seen, new_len * sizeof(bool));
    if (!new_seen) exit(1);
    ans_val   = new_val;
    ans_seen  = new_seen;
    for (size_t i = ans_cache_len; i < new_len; ++i) ans_seen[i] = false;
    ans_cache_len = new_len;
}

/* test whether decimal representation of 'hay' contains that of
   'need' as a proper substring (hay must be strictly longer)         */
static bool seq_contains_substring(seq_t hay, seq_t need)
{
    /* 8192‑digit buffer is sufficient for all Fibonacci numbers up
       to index 10 000 (F(10000) has about 2089 digits).               */
    static char buf_h[8192];
    static char buf_n[8192];
    size_t len_h, len_n;

    seq_to_str(hay, buf_h, &len_h);
    seq_to_str(need, buf_n, &len_n);
    buf_h[len_h] = '\0';
    buf_n[len_n] = '\0';

    if (len_h <= len_n) return false;          /* proper substring required */
    return strstr(buf_h, buf_n) != NULL;
}

/* ------------------------------------------------------------------ */
/*                     Main sequence function                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* return cached result if already known */
    ensure_ans_cache(n);
    if (ans_seen[n]) return ans_val[n];

    /* ---------------------------------------------------- */
    /*  Fibonacci iteration to find target and search       */
    /* ---------------------------------------------------- */
    seq_t fprev = (seq_t)0;   /* F(0) */
    seq_t fcurr = (seq_t)1;   /* F(1) */
    idx_t idx   = 1;          /* current index (fcurr == F(idx)) */

    /* advance until idx > n, leaving fprev = F(n-1), fcurr = F(n) */
    while (idx <= n) {
        seq_t fnext = fprev + fcurr;
        fprev = fcurr;
        fcurr = fnext;
        ++idx;
    }
    /* at this point fprev == F(n),   fcurr == F(n+1) */
    seq_t target = fprev;     /* Fibonacci(n) */

    /* now search from F(n+1) upwards for the first containing target */
    while (1) {
        if (seq_contains_substring(fcurr, target)) {
            ans_val[n] = fcurr;
            ans_seen[n] = true;
            return fcurr;
        }
        /* step forward */
        seq_t fnext = fprev + fcurr;
        fprev = fcurr;
        fcurr = fnext;
        ++idx;
    }
}
