/*
 * A251465  Number of (n+1) X (6+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to a prime and those sums nondecreasing in every row and
 *          column.
 * https://oeis.org/A251465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 arrays with entries 0..2 * such that
 *            each 2×2 subblock sums to a prime (2,3,5,7) and the * 2×2 block
 *            sums are nondecreasing in every row and every column
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 30, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Constants ----------------------------------------------------------- */
/*---------------------------------------------------------------------*/
#define ROWS          2187U   /* 3^7 */
#define COLS          7U
#define MONOLEN       6U      /* number of 2×2 sums in a row pair */
#define PRIME_CNT     4U      /* {2,3,5,7} */
#define MONO_COUNT    84U     /* number of non‑decreasing 6‑element vectors
                               with values 0..3 (representing 2,3,5,7) */
#define TOTAL_STATES  (ROWS * MONO_COUNT)

/* --------------------------------------------------------------------- */
/* Forward declarations ----------------------------------------------- */
/*---------------------------------------------------------------------*/
static void init_preprocess(void);
static int  prime_index[9];
static uint8_t mono_code_to_index[4096];
static uint16_t mono_codes[MONO_COUNT];
static uint8_t compGE[MONO_COUNT][MONO_COUNT];
static uint16_t *trans_row_copy;   /* transition source row (x) */
static uint8_t  *trans_next_index;/* transition target sum vector index   */
static size_t   *trans_start;      /* start indices for each source row    */

/* --------------------------------------------------------------------- */
/* State arrays -------------------------------------------------------- */
/*---------------------------------------------------------------------*/
static seq_t  *cur_counts = NULL;
static seq_t  *next_counts = NULL;
static seq_t  *value_cache = NULL;   /* a(n) values (1‑based) */
static idx_t   cache_size = 0;
static idx_t   last_computed = 0;

/* --------------------------------------------------------------------- */
/* Helpers ------------------------------------------------------------- */
/*---------------------------------------------------------------------*/
static uint8_t row_digits[ROWS][COLS];
static int     init_done = 0;

/* --------------------------------------------------------------------- */
/* Preprocess ---------------------------------------------------------- */
/* ---------------------------------------------------------------------*/
static void init_preprocess(void)
{
    if (init_done) return;
    init_done = 1;

    /* 1.  Map sums 0..8 to prime indices 0..3 (for 2,3,5,7). */
    for (int i = 0; i < 9; ++i) prime_index[i] = -1;
    prime_index[2] = 0;   /* 2 */
    prime_index[3] = 1;   /* 3 */
    prime_index[5] = 2;   /* 5 */
    prime_index[7] = 3;   /* 7 */

    /* 2.  Decode all 2187 rows into 7‑digit base‑3 arrays. */
    for (int row = 0; row < ROWS; ++row) {
        int tmp = row;
        for (uint8_t c = 0; c < COLS; ++c) {
            row_digits[row][c] = tmp % 3;
            tmp /= 3;
        }
    }

    /* 3.  Enumerate all 6‑digit base‑4 vectors whose values are
       non‑decreasing. Keep mapping from code to compact index. */
    memset(mono_code_to_index, 0xFF, sizeof(mono_code_to_index));
    int mono_cnt = 0;
    for (uint16_t code = 0; code < (1U << 12); ++code) {
        uint16_t tmp = code;
        int prev_val = -1;
        bool ok = true;
        for (uint8_t k = 0; k < MONOLEN; ++k) {
            uint8_t p = tmp & 3U;
            int val   = (p == 0) ? 2 : (p == 1) ? 3 : (p == 2) ? 5 : 7;
            if (prev_val > val) { ok = false; break; }
            prev_val = val;
            tmp >>= 2;
        }
        if (ok) {
            mono_codes[mono_cnt] = code;
            mono_code_to_index[code] = (uint8_t)mono_cnt;
            ++mono_cnt;
        }
    }
    (void)mono_cnt; /* silence unused warning */

    /* 4.  Precompute component‑wise “greater or equal” table. */
    for (uint8_t i = 0; i < MONO_COUNT; ++i) {
        for (uint8_t j = 0; j < MONO_COUNT; ++j) {
            uint16_t ci = mono_codes[i];
            uint16_t cj = mono_codes[j];
            bool ge = true;
            for (uint8_t k = 0; k < MONOLEN; ++k) {
                uint8_t vi = (ci >> (2 * k)) & 3U;
                uint8_t vj = (cj >> (2 * k)) & 3U;
                if (vj < vi) { ge = false; break; }
            }
            compGE[i][j] = ge ? 1 : 0;
        }
    }

    /* 5.  Build all admissible transitions from every row X to all
           rows Y.  For each X store the list of Y and the target
           sum‑vector index. */
    size_t *row_transition_count = (size_t *)calloc(ROWS, sizeof(size_t));
    size_t total_transitions = 0;
    for (int x = 0; x < ROWS; ++x) {
        const uint8_t *xd = row_digits[x];
        for (int y = 0; y < ROWS; ++y) {
            const uint8_t *yd = row_digits[y];
            uint16_t sumCode = 0;
            bool ok = true;
            int prevP = -1;
            for (uint8_t k = 0; k < MONOLEN; ++k) {
                int sum = xd[k] + xd[k + 1] + yd[k] + yd[k + 1];
                int p = prime_index[sum];
                if (p < 0) { ok = false; break; }
                if (prevP >= 0 && p < prevP) { ok = false; break; }
                sumCode |= ((uint16_t)p) << (2 * k);  /* low bits = first element */
                prevP = p;
            }
            if (ok) {
                uint8_t idx = mono_code_to_index[sumCode];
                if (idx != 0xFF) {
                    ++row_transition_count[x];
                }
            }
        }
    }
    for (int x = 0; x < ROWS; ++x) total_transitions += row_transition_count[x];

    trans_start = (size_t *)malloc((ROWS + 1) * sizeof(size_t));
    trans_row_copy = (uint16_t *)malloc(total_transitions * sizeof(uint16_t));
    trans_next_index = (uint8_t *)malloc(total_transitions * sizeof(uint8_t));

    trans_start[0] = 0;
    for (int x = 0; x < ROWS; ++x)
        trans_start[x + 1] = trans_start[x] + row_transition_count[x];

    /* fill transition arrays */
    memset(row_transition_count, 0, ROWS * sizeof(size_t));  /* reuse as counter */
    for (int x = 0; x < ROWS; ++x) {
        const uint8_t *xd = row_digits[x];
        size_t base = trans_start[x];
        size_t pos  = 0;
        for (int y = 0; y < ROWS; ++y) {
            const uint8_t *yd = row_digits[y];
            uint16_t sumCode = 0;
            bool ok = true;
            int prevP = -1;
            for (uint8_t k = 0; k < MONOLEN; ++k) {
                int sum = xd[k] + xd[k + 1] + yd[k] + yd[k + 1];
                int p = prime_index[sum];
                if (p < 0) { ok = false; break; }
                if (prevP >= 0 && p < prevP) { ok = false; break; }
                sumCode |= ((uint16_t)p) << (2 * k);
                prevP = p;
            }
            if (ok) {
                uint8_t idx = mono_code_to_index[sumCode];
                if (idx != 0xFF) {
                    size_t idxPos = base + pos;
                    trans_row_copy[idxPos]   = (uint16_t)y;
                    trans_next_index[idxPos] = idx;
                    ++pos;
                }
            }
        }
    }
    free(row_transition_count);

    /* 6.  Allocate DP arrays for state counts. */
    cur_counts = (seq_t *)calloc(TOTAL_STATES, sizeof(seq_t));
    next_counts = (seq_t *)calloc(TOTAL_STATES, sizeof(seq_t));

    /* 7.  Allocate value cache (a(0) unused). */
    cache_size = 2U;                /* at least index 0 and 1 */
    value_cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    value_cache[0] = 0;             /* unused */
}

/* --------------------------------------------------------------------- */
/* Helper: ensure cache capacity --------------------------------------- */
/* ---------------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    if ((idx_t)(n + 1) > cache_size) {
        idx_t newSize = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(value_cache, newSize * sizeof(seq_t));
        if (!tmp) exit(1);
        for (idx_t i = cache_size; i < newSize; ++i) tmp[i] = 0;
        value_cache = tmp;
        cache_size = newSize;
    }
}

/* --------------------------------------------------------------------- */
/* Compute values up to n ------------------------------------------------
   -----------------------------------------------------------*/
static void compute_to(idx_t n)
{
    while (last_computed < n) {
        ++last_computed;   /* compute a(last_computed) */

        if (last_computed == 1) {
            /* first pair of rows */
            memset(cur_counts, 0, TOTAL_STATES * sizeof(seq_t));
            for (int x = 0; x < ROWS; ++x) {
                size_t start = trans_start[x];
                size_t end   = trans_start[x + 1];
                for (size_t t = start; t < end; ++t) {
                    uint16_t y   = trans_row_copy[t];
                    uint8_t  sIdx = trans_next_index[t];
                    size_t idx = (size_t)y * MONO_COUNT + sIdx;
                    cur_counts[idx] += 1;
                }
            }
            /* total for n=1 */
            seq_t total = 0;
            for (size_t i = 0; i < TOTAL_STATES; ++i)
                total += cur_counts[i];
            value_cache[1] = total;
            continue;
        }

        /* subsequent rows */
        memset(next_counts, 0, TOTAL_STATES * sizeof(seq_t));

        for (size_t state = 0; state < TOTAL_STATES; ++state) {
            seq_t curVal = cur_counts[state];
            if (!curVal) continue;
            uint16_t rowX = (uint16_t)(state / MONO_COUNT);
            uint8_t  sIdxPrev = (uint8_t)(state % MONO_COUNT);
            size_t start = trans_start[rowX];
            size_t end   = trans_start[rowX + 1];
            for (size_t t = start; t < end; ++t) {
                if (!compGE[sIdxPrev][trans_next_index[t]]) continue;
                uint16_t yIdx = trans_row_copy[t];
                uint8_t  sIdxNext = trans_next_index[t];
                size_t nextState = (size_t)yIdx * MONO_COUNT + sIdxNext;
                next_counts[nextState] += curVal;
            }
        }

        /* compute total for current n */
        seq_t total = 0;
        for (size_t i = 0; i < TOTAL_STATES; ++i)
            total += next_counts[i];
        value_cache[last_computed] = total;

        /* swap count arrays */
        seq_t *tmp = cur_counts;
        cur_counts = next_counts;
        next_counts = tmp;
    }
}

/* --------------------------------------------------------------------- */
/* Public function ----------------------------------------------------- */
/* ---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_preprocess();
    ensure_cache(n);
    if (value_cache[n] == 0) compute_to(n);
    return value_cache[n];
}
