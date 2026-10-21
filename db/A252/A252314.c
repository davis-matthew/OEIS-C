/*
 * A252314  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 22, u64 n <= 52, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A252314
    Number of (n+2)×3 arrays with entries 0..3 where every 3×3 subblock
    has row, column, major‑diagonal and anti‑diagonal sums not equal
    to 0, 1, 3, 6 or 7.
    a(n) = the number of such arrays for n+2 rows (offset 1).
    The function below computes a(n) for any n ≥ 1 using a dynamic
    programming approach that is O(n · S²) with S = number of
    admissible rows (≈30).  All intermediate values are kept in seq_t
    to avoid overflow.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>  /* memset */

/* ---------- constants ---------- */
#define DIGITS 3                  /* width of the array */
#define ROWS 64                   /* 4³ possible rows */
#define FORBIDDEN_SUMS 5          /* number of forbidden sums */

/* ---------- forbidden sums ---------- */
static const int forbidden[10] = { /* 0..9 */
    1, /* 0 forbidden */
    1, /* 1 forbidden */
    0, /* 2 allowed  */
    1, /* 3 forbidden */
    0, /* 4 allowed  */
    0, /* 5 allowed  */
    1, /* 6 forbidden */
    1, /* 7 forbidden */
    0, /* 8 allowed  */
    0  /* 9 allowed  */
};

/* ---------- state data (initialized in init()) ---------- */
static size_t allowed_cnt = 0;
static uint8_t  allowed_rows[ROWS];         /* indices of allowed rows (0..63) */
static uint8_t  row_digits[ROWS][DIGITS];   /* digits of all rows 0..63 */

static size_t S = 0;                     /* number of states = allowed_cnt² */
static uint16_t *trans_start = NULL;      /* start index in trans_next for each state   */
static uint16_t *trans_next  = NULL;      /* flattened list of next states              */
static uint16_t *trans_count = NULL;      /* number of transitions per state           */

static seq_t *dp_cur = NULL;              /* DP for current number of rows            */
static seq_t *dp_nxt = NULL;              /* DP for next number of rows               */

/* ---------- memoization ---------- */
#define MAX_N 10000                               /* maximum n we expect to receive      */
static seq_t *terms = NULL;                      /* a(1)...a(MAX_N) (1‑based) */
static idx_t terms_computed = 0;  /* number of terms already stored (i.e. highest n computed)*/
static size_t current_level = 2;   /* current number of rows represented by dp_cur */

/* ---------- helper to initialise everything ---------- */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* --- collect digits of all 64 possible rows --- */
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        for (int d = 0; d < DIGITS; ++d) {
            row_digits[r][d] = v & 3;
            v >>= 2;
        }
        int sum = row_digits[r][0] + row_digits[r][1] + row_digits[r][2];
        if (!forbidden[sum]) {
            allowed_rows[allowed_cnt++] = (uint8_t)r;
        }
    }

    /* --- number of states --- */
    S = allowed_cnt * allowed_cnt;

    /* --- pre‑compute transitions --- */
    trans_count = (uint16_t*)calloc(S, sizeof(uint16_t));
    /* first pass: count transitions */
    for (size_t a = 0; a < allowed_cnt; ++a) {
        uint8_t *da = row_digits[allowed_rows[a]];
        for (size_t b = 0; b < allowed_cnt; ++b) {
            uint8_t *db = row_digits[allowed_rows[b]];
            size_t state = a * allowed_cnt + b;
            for (size_t c = 0; c < allowed_cnt; ++c) {
                uint8_t *dc = row_digits[allowed_rows[c]];
                /* check the 3×3 block formed by rows a,b,c */
                int col0 = da[0] + db[0] + dc[0];
                int col1 = da[1] + db[1] + dc[1];
                int col2 = da[2] + db[2] + dc[2];
                if (forbidden[col0] || forbidden[col1] || forbidden[col2]) continue;

                int diag  = da[0] + db[1] + dc[2];
                int anti  = da[2] + db[1] + dc[0];
                if (forbidden[diag] || forbidden[anti]) continue;

                trans_count[state]++;
            }
        }
    }

    /* total number of transition entries */
    size_t total_trans = 0;
    for (size_t s = 0; s < S; ++s) total_trans += trans_count[s];

    trans_start = (uint16_t*)malloc(S * sizeof(uint16_t));
    trans_next  = (uint16_t*)malloc(total_trans * sizeof(uint16_t));
    /* second pass: fill transitions */
    trans_start[0] = 0;
    for (size_t s = 1; s < S; ++s) {
        trans_start[s] = trans_start[s-1] + trans_count[s-1];
    }
    /* temporary counter for filling each state segment */
    uint16_t *idx_counter = (uint16_t*)calloc(S, sizeof(uint16_t));
    for (size_t a = 0; a < allowed_cnt; ++a) {
        uint8_t *da = row_digits[allowed_rows[a]];
        for (size_t b = 0; b < allowed_cnt; ++b) {
            uint8_t *db = row_digits[allowed_rows[b]];
            size_t state = a * allowed_cnt + b;
            for (size_t c = 0; c < allowed_cnt; ++c) {
                uint8_t *dc = row_digits[allowed_rows[c]];
                int col0 = da[0] + db[0] + dc[0];
                int col1 = da[1] + db[1] + dc[1];
                int col2 = da[2] + db[2] + dc[2];
                if (forbidden[col0] || forbidden[col1] || forbidden[col2]) continue;
                int diag  = da[0] + db[1] + dc[2];
                int anti  = da[2] + db[1] + dc[0];
                if (forbidden[diag] || forbidden[anti]) continue;
                size_t next_state = b * allowed_cnt + c;
                size_t pos = trans_start[state] + idx_counter[state]++;
                trans_next[pos] = (uint16_t)next_state;
            }
        }
    }
    free(idx_counter);

    /* --- allocate DP arrays --- */
    dp_cur = (seq_t*)calloc(S, sizeof(seq_t));
    dp_nxt = (seq_t*)calloc(S, sizeof(seq_t));

    /* initialise dp_cur with all pairs of allowed rows (height=2) */
    for (size_t a = 0; a < allowed_cnt; ++a) {
        for (size_t b = 0; b < allowed_cnt; ++b) {
            dp_cur[a * allowed_cnt + b] = (seq_t)1;
        }
    }

    /* --- allocate terms array --- */
    terms = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));  /* terms[0] unused */
    terms[0] = 0;
    terms_computed = 0;
    current_level = 2;   /* dp_cur represents height=2 rows */
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    init();                         /* ensure tables are built */

    /* compute terms up to requested n */
    while (terms_computed < n) {
        /* transition from current to next level (add one row) */
        memset(dp_nxt, 0, S * sizeof(seq_t));

        for (size_t s = 0; s < S; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            uint16_t start = trans_start[s];
            uint16_t cnt   = trans_count[s];
            for (uint16_t t = 0; t < cnt; ++t) {
                uint16_t next_s = trans_next[start + t];
                dp_nxt[next_s] += val;
            }
        }

        /* swap dp_cur and dp_nxt for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;

        current_level++;   /* number of rows now represented */

        /* compute total number of arrays of height = current_level */
        seq_t total = (seq_t)0;
        for (size_t s = 0; s < S; ++s) {
            total += dp_cur[s];
        }

        /* store the term (n = current_level - 2) */
        terms[++terms_computed] = total;
    }

    return terms[n];
}
