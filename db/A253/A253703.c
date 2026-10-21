/*
 * A253703  Number of (6+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference unequal to its neighbors
 *          horizontally and nw+se diagonal sum unequal to its neighbors
 *          vertically.
 * https://oeis.org/A253703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined recursively for n > 31 by a(n) = 2*a(n-2) +
 *            a(n-5) - a(n-6) + 2*a(n-7) + a(n-8) - 8*a(n-9) - a(n-10) +
 *            2*a(n-11) + 5*a(n-13) - 11*a(n-15) + 6*a(n-17) + 7*a(n-19) -
 *            4*a(n-21). Initial values a(1) … a(31) are computed by exact
 *            counting with a dynamic programming algorithm described...
 * Range:     u8 none, u16 n <= 16, u32 n <= 51, u64 n <= 122, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Number of rows in the array (6+1) */
#define NUM_ROWS 7
/* Number of possible 2×2 blocks vertically (rows-1) */
#define NUM_BLOCKS (NUM_ROWS-1)
/* Number of possible column bit patterns (7 bits) */
#define NUM_COLS 128
/* Number of difference codes: 3^(NUM_BLOCKS)+1 for a sentinel */
#define NUM_DIFF_STATES ((1 + 3 * 3 * 3 * 3 * 3 * 3) + 1)   /* 730 */
#define SENTINEL (NUM_DIFF_STATES-1)                       /* 729 */

/* Tables for DP */
static unsigned char  vertical_ok[NUM_COLS][NUM_COLS];
static unsigned short dcur_code[NUM_COLS][NUM_COLS];
static signed char    diff_digits[NUM_DIFF_STATES-1][NUM_BLOCKS];
static unsigned char  horizontal_ok[NUM_DIFF_STATES-1][NUM_DIFF_STATES-1];

/* DP arrays */
static seq_t *dp_curr = NULL;
static seq_t *dp_next = NULL;

/* Cache for sequence values */
static seq_t *cache   = NULL;
static idx_t  cache_size = 0;

/* First 31 terms (index 1..31) after DP */
static seq_t first_terms[NUM_DIFF_STATES];

/* ---------------------------------------------------------------------- */
/*  Initialization of tables and DP to compute the first 31 terms       */
/* ---------------------------------------------------------------------- */
static void init_cache(void)
{
    if (cache_size != 0) return;   /* already initialised */

    /* Pre–compute powers of 3 for base-3 encoding of diff vectors (length 6) */
    int pow3[NUM_BLOCKS];
    pow3[0] = 1;
    for (int i = 1; i < NUM_BLOCKS; ++i) pow3[i] = pow3[i-1] * 3;

    /* Pre–compute vertical_ok and diff code for all column pairs */
    for (int prev = 0; prev < NUM_COLS; ++prev) {
        for (int next = 0; next < NUM_COLS; ++next) {
            int sum_prev = -1, sum_cur, ok = 1;
            int code = 0;
            for (int i = 0; i < NUM_BLOCKS; ++i) {
                int bit_prev_i  = (prev >> i) & 1;
                int bit_next_ip1 = (next >> (i+1)) & 1;

                sum_cur = bit_prev_i + bit_next_ip1;
                if (i > 0 && sum_cur == sum_prev) { ok = 0; break; }
                sum_prev = sum_cur;

                /* Difference for the (NE‑SW) antidiagonal of this 2×2 block */
                int bit_next_i   = (next >> i) & 1;
                int bit_prev_ip1 = (prev >> (i+1)) & 1;
                int diff = bit_next_i - bit_prev_ip1;    /* -1,0,1 */
                code += (diff + 1) * pow3[i];
            }
            vertical_ok[prev][next] = (unsigned char)ok;
            dcur_code[prev][next]   = (unsigned short)code;
        }
    }

    /* Decode diff_codes into digits (-1,0,1) */
    for (int code = 0; code < NUM_DIFF_STATES-1; ++code) {
        int tmp = code;
        for (int i = 0; i < NUM_BLOCKS; ++i) {
            int digit = tmp % 3;                /* 0,1,2 */
            diff_digits[code][i] = (signed char)(digit - 1);
            tmp /= 3;
        }
    }

    /* Pre‑compute horizontal_ok : two diff vectors are “allowed” if all
       corresponding components differ (ne‑sw anti‑diagonal difference must be
       distinct from its horizontal neighbour). */
    for (int code1 = 0; code1 < NUM_DIFF_STATES-1; ++code1) {
        for (int code2 = 0; code2 < NUM_DIFF_STATES-1; ++code2) {
            unsigned char ok = 1;
            for (int i = 0; i < NUM_BLOCKS; ++i) {
                if (diff_digits[code1][i] == diff_digits[code2][i]) {
                    ok = 0; break;
                }
            }
            horizontal_ok[code1][code2] = ok;
        }
    }

    /* Allocate DP arrays: number of states = NUM_COLS * NUM_DIFF_STATES
       (including sentinel). */
    size_t states = (size_t)NUM_COLS * NUM_DIFF_STATES;
    dp_curr = (seq_t *)calloc(states, sizeof(seq_t));
    dp_next = (seq_t *)calloc(states, sizeof(seq_t));

    /* Initialise DP for the first column: no constraints yet ⇒ sentinel diff. */
    for (int col = 0; col < NUM_COLS; ++col) {
        size_t idx = (size_t)col * NUM_DIFF_STATES + SENTINEL;
        dp_curr[idx] = 1;
    }

    /* DP for width 2 .. 32  (n = 1 .. 31) */
    for (int step = 1; step <= 31; ++step) {
        memset(dp_next, 0, states * sizeof(seq_t));

        for (size_t idx = 0; idx < states; ++idx) {
            seq_t cnt = dp_curr[idx];
            if (!cnt) continue;

            int prev_col   = (int)(idx / NUM_DIFF_STATES);
            int dprev_code = (int)(idx % NUM_DIFF_STATES);

            for (int next_col = 0; next_col < NUM_COLS; ++next_col) {
                if (!vertical_ok[prev_col][next_col]) continue;

                int dcur = (int)dcur_code[prev_col][next_col];
                if (dprev_code != SENTINEL && !horizontal_ok[dprev_code][dcur])
                    continue;

                size_t new_idx = (size_t)next_col * NUM_DIFF_STATES + (size_t)dcur;
                dp_next[new_idx] += cnt;
            }
        }

        /* Sum all states to obtain a(step) for width step+1 (i.e., n = step). */
        seq_t total = 0;
        for (size_t idx = 0; idx < states; ++idx) total += dp_next[idx];
        first_terms[step] = total;

        /* Prepare for next iteration */
        seq_t *tmp = dp_curr;
        dp_curr  = dp_next;
        dp_next  = tmp;
    }

    /* Initialise cache with first 31 values (indices 1..31) */
    cache_size = 32;   /* indices 0..31 */
    cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
    for (idx_t i = 1; i <= 31; ++i) cache[i] = first_terms[i];
}

/* ---------------------------------------------------------------------- */
/*  Extend cache up to index n using the recurrence (n > 31)             */
/* ---------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    size_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    memset(cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    cache_size = new_size;

    for (idx_t i = 32; i <= n; ++i) {
        seq_t pos = 0;
        pos += 2 * cache[i-2] + cache[i-5] + 2 * cache[i-7] +
               cache[i-8] + 2 * cache[i-11] + 5 * cache[i-13] +
               6 * cache[i-17] + 7 * cache[i-19];

        seq_t neg = 0;
        neg += cache[i-6] + 8 * cache[i-9] + cache[i-10] +
               11 * cache[i-15] + 4 * cache[i-21];

        cache[i] = pos - neg;    /* guaranteed to be positive */
    }
}

/* ---------------------------------------------------------------------- */
/*  Public sequence function                                             */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_cache();
    if (n <= 31) return cache[n];
    ensure_cache(n);
    return cache[n];
}
