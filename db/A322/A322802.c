/*
 * A322802  Number of compositions (ordered partitions) of n into centered
 *          hexagonal numbers (A003215).
 * https://oeis.org/A322802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A322802: Number of compositions of n into centered hexagonal numbers.
 * Formula: a(0) = 1
 * a(n) = Σ a(n - (3*k*(k+1)+1))   for all k >= 0 with 3*k*(k+1)+1 <= n
 *
 * The implementation uses dynamic programming and memoisation to compute
 * values on demand.  It works for any idx_t and seq_t provided by the
 * build system and remains fast for n up to at least 10000.
 */

#include <stdlib.h>
#include <stddef.h>

/* Forward declaration of the required function */
seq_t a(idx_t n);

/* -------------------------------------------------------------------------- */
/*  Helper data structures                                                     */
/* -------------------------------------------------------------------------- */

static seq_t *dp = NULL;          /* DP table: dp[i] = a(i)                */
static size_t dp_alloc = 0;       /* Allocated size of dp                  */
static size_t dp_size  = 0;       /* Number of computed entries (dp[0..dp_size-1]) */

static idx_t *hexs = NULL;        /* Array of centered hexagonal numbers   */
static size_t hex_cap = 0;        /* Allocated size of hexs                */
static size_t hex_len = 0;        /* Number of elements in hexs            */

/* -------------------------------------------------------------------------- */
/*  Ensure that 'hexs' contains all centered hexagonal numbers <= n          */
/* -------------------------------------------------------------------------- */

static void ensure_hexs(idx_t n)
{
    /* Expand hexs array until the next centered hex value exceeds n */
    while (1) {
        idx_t k = (idx_t)hex_len;                      /* next k   */
        idx_t val = 3*k*(k+1) + 1;                     /* 3k(k+1)+1 */

        if (val > n) break;                            /* nothing more needed */

        if (hex_len == hex_cap) {                      /* grow array   */
            size_t new_cap = hex_cap ? hex_cap * 2 : 1;
            idx_t *tmp = (idx_t *)realloc(hexs, new_cap * sizeof(idx_t));
            if (!tmp) abort();                         /* allocation failure */
            hexs = tmp;
            hex_cap = new_cap;
        }

        hexs[hex_len++] = val;                         /* store value  */
    }
}

/* -------------------------------------------------------------------------- */
/*  Compute dp values up to n (inclusive)                                     */
/* -------------------------------------------------------------------------- */

static void ensure_dp(idx_t n)
{
    size_t target = (size_t)n + 1;                     /* needed size */

    /* Grow dp array if necessary */
    if (dp_alloc < target) {
        size_t new_alloc = dp_alloc ? dp_alloc * 2 : 1;
        while (new_alloc < target) new_alloc <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_alloc * sizeof(seq_t));
        if (!tmp) abort();                            /* allocation failure */
        dp = tmp;
        dp_alloc = new_alloc;
    }

    /* Initialise the new part of dp to zero before computing */
    for (size_t i = dp_size; i < target; ++i) dp[i] = 0;

    /* Base case: a(0) = 1 */
    if (dp_size == 0) {
        dp[0] = 1;
        dp_size = 1;
    }

    /* Compute missing dp entries */
    for (size_t i = dp_size; i < target; ++i) {
        seq_t sum = 0;
        for (size_t p = 0; p < hex_len; ++p) {
            idx_t s = hexs[p];
            if (s > i) break;
            sum += dp[i - s];
        }
        dp[i] = sum;
    }

    dp_size = target;          /* all values up to n are now computed */
}

/* -------------------------------------------------------------------------- */
/*  Public function                                                            */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    ensure_hexs(n);                 /* make sure hexs has all needed parts */
    ensure_dp(n);                   /* compute dp[n] if not yet done */
    return dp[(size_t)n];
}
