/*
 * A251985  Number of (n+2)X(1+2) 0..2 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 1 or 5.
 * https://oeis.org/A251985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+2)×3 arrays with entries 0..2 such that in
 *            every 3×3 subblock no row, column, diagonal or antidiagonal sums
 *            to 1 or 5
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* internal structures */
#define ROWS_PER_VAL 27       /* 3^3 possible rows */
#define MAX_VALID_ROWS 27     /* at most this many valid rows */
#define MAX_STATES    (MAX_VALID_ROWS * MAX_VALID_ROWS)

/* tables for precomputation */
static unsigned char row_digits[ROWS_PER_VAL][3];
static bool row_invalid[ROWS_PER_VAL];
static unsigned char valid_rows[MAX_VALID_ROWS];
static size_t valid_count;

/* transition lists: for each state (p1,p2) list of next valid row indices */
static unsigned char trans_next[MAX_STATES][MAX_VALID_ROWS];
static unsigned char trans_count[MAX_STATES];
static size_t state_count;

/* DP arrays */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* cache of results a(n) */
static seq_t *a_cache = NULL;
static size_t cache_size = 0;
static size_t cache_max_n = 0;      /* how many n's computed (max index) */

/* current length represented by dp_cur (number of rows processed) */
static size_t cur_len = 2;          /* start with 2 rows (no 3×3 subblock yet) */

/* initialization of tables – executed once */
static void init_tables(void)
{
    /* build row digits and detect invalid rows (sum 1 or 5) */
    for (size_t i = 0; i < ROWS_PER_VAL; ++i) {
        row_digits[i][0] = (unsigned char)(i / 9);
        row_digits[i][1] = (unsigned char)((i % 9) / 3);
        row_digits[i][2] = (unsigned char)(i % 3);
        unsigned int sum = row_digits[i][0] + row_digits[i][1] + row_digits[i][2];
        row_invalid[i] = (sum == 1 || sum == 5);
    }

    /* collect valid rows */
    valid_count = 0;
    for (size_t i = 0; i < ROWS_PER_VAL; ++i)
        if (!row_invalid[i])
            valid_rows[valid_count++] = (unsigned char)i;

    state_count = valid_count * valid_count;

    /* build transition lists */
    for (size_t a = 0; a < valid_count; ++a)
        for (size_t b = 0; b < valid_count; ++b) {
            size_t state = a * valid_count + b;
            size_t cnt = 0;
            unsigned char ia = valid_rows[a];
            unsigned char ib = valid_rows[b];
            for (size_t c = 0; c < valid_count; ++c) {
                unsigned char ic = valid_rows[c];
                /* Check column sums */
                bool bad = false;
                for (int col = 0; col < 3; ++col) {
                    unsigned int cs = row_digits[ia][col] +
                                      row_digits[ib][col] +
                                      row_digits[ic][col];
                    if (cs == 1 || cs == 5) { bad = true; break; }
                }
                if (bad) continue;
                /* Diagonals */
                unsigned int diag1 = row_digits[ia][0] +
                                     row_digits[ib][1] +
                                     row_digits[ic][2];
                if (diag1 == 1 || diag1 == 5) continue;
                unsigned int diag2 = row_digits[ia][2] +
                                     row_digits[ib][1] +
                                     row_digits[ic][0];
                if (diag2 == 1 || diag2 == 5) continue;
                /* Transition allowed */
                trans_next[state][cnt++] = (unsigned char)c;
            }
            trans_count[state] = (unsigned char)cnt;
        }

    /* allocate DP arrays */
    dp_cur = (seq_t *)malloc(state_count * sizeof(seq_t));
    dp_next = (seq_t *)malloc(state_count * sizeof(seq_t));
    /* initialize dp_cur for length 2: every pair of valid rows is allowed */
    for (size_t s = 0; s < state_count; ++s)
        dp_cur[s] = (seq_t)1;

    /* initial cache (n=0, length=2) – not used but convenient */
    cache_size = 1;
    a_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    a_cache[0] = (seq_t)state_count;  /* number of valid rows squared */
}

/* ensure cache can hold at least n terms */
static void ensure_cache(size_t need)
{
    if (need < cache_size) return;
    size_t new_cap = cache_size == 0 ? 1 : cache_size;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *newc = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!newc) abort();  /* allocation failure */
    a_cache = newc;
    cache_size = new_cap;
}

/* main function required by OEIS */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, but safe to return 0 */

    static bool built = false;
    if (!built) { init_tables(); built = true; }

    /* grow to required n */
    if ((size_t)n > cache_max_n) {
        size_t target_len = (size_t)n + 2;   /* m = n+2 rows */
        while (cur_len < target_len) {
            /* clear next */
            for (size_t i = 0; i < state_count; ++i) dp_next[i] = (seq_t)0;

            /* compute next DP step */
            for (size_t s = 0; s < state_count; ++s) {
                seq_t val = dp_cur[s];
                if (val == (seq_t)0) continue;
                unsigned int base = (unsigned int)(s % valid_count) * valid_count;
                size_t cnt = trans_count[s];
                for (size_t k = 0; k < cnt; ++k) {
                    size_t ns = base + trans_next[s][k];
                    dp_next[ns] = dp_next[ns] + val;
                }
            }

            /* swap dp arrays */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
            ++cur_len;

            /* compute and store result for this length */
            idx_t cur_n = (idx_t)(cur_len - 2);  /* because cur_len = n+2 */
            ensure_cache((size_t)cur_n);
            seq_t total = (seq_t)0;
            for (size_t i = 0; i < state_count; ++i) total = total + dp_cur[i];
            a_cache[cur_n] = total;
        }
        cache_max_n = (size_t)n;
    }

    return a_cache[(size_t)n];
}
