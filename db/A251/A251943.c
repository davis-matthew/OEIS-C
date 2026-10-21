/*
 * A251943  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum prime and every diagonal and antidiagonal sum nonprime.
 * https://oeis.org/A251943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 0..3 arrays with every 3×3 subblock row
 *            and column sum prime and every diagonal and antidiagonal sum
 *            nonprime. This implementation uses a transfer‑matrix DP over
 *            pairs of rows
 * Range:     u8 none, u16 n <= 10, u32 n <= 29, u64 n <= 68, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define ROWS          64            /* 4³ possible rows */
#define STATES        (ROWS * ROWS) /* pair of rows */

/* primality for sums 0..9 */
static const bool small_prime[10] = {
    false, false, true, true, false, true,
    false, true, false, false
};

/* digits of each row (base‑4 representation) */
static uint8_t row_dig[ROWS][3];
/* whether the row sum is prime */
static bool   row_prime[ROWS];

/* for each state we store the starting index in the flat
   destination list and the number of outgoing edges. */
static int   *trans_start;   /* size STATES + 1 */
static int   *trans_list;    /* flat list of destination states */

/* DP vectors for the current and next length */
static seq_t *dp_cur;
static seq_t *dp_next;

/* cache of sequence values: a_values[n] holds a(n) for n ≥ 1 */
static seq_t *a_values;
static idx_t  a_len = 0;     /* current allocated size for a_values (≥ n+1) */

/* current maximum n that has been computed */
static idx_t max_n_computed = 0;
static bool   initialized = false;

/* ------------------------------------------------------------------------- */
static bool block_ok(int a, int b, int c)
{
    /* column sums must be prime */
    for (int j = 0; j < 3; ++j) {
        int sum = row_dig[a][j] + row_dig[b][j] + row_dig[c][j];
        if (!small_prime[sum]) return false;
    }
    /* main diagonal and anti‑diagonal must be nonprime */
    int diag1 = row_dig[a][0] + row_dig[b][1] + row_dig[c][2];
    int diag2 = row_dig[a][2] + row_dig[b][1] + row_dig[c][0];
    if (small_prime[diag1] || small_prime[diag2]) return false;
    return true;
}

/* ------------------------------------------------------------------------- */
static void init(void)
{
    size_t i, j, k;
    /* precompute row digits and prime sums */
    for (i = 0; i < ROWS; ++i) {
        row_dig[i][0] = i & 3;
        row_dig[i][1] = (i >> 2) & 3;
        row_dig[i][2] = (i >> 4) & 3;
        int s = row_dig[i][0] + row_dig[i][1] + row_dig[i][2];
        row_prime[i] = small_prime[s];
    }

    /* allocate transition arrays */
    trans_start = (int*)malloc((STATES + 1) * sizeof(int));
    trans_list  = (int*)malloc(STATES * 64 * sizeof(int)); /* rough upper bound */

    int *state_deg = (int*)malloc(STATES * sizeof(int));
    int total_deg = 0;
    for (i = 0; i < ROWS; ++i) {
        if (!row_prime[i]) continue;
        for (j = 0; j < ROWS; ++j) {
            if (!row_prime[j]) continue;
            int state = (int)(i * ROWS + j);
            int deg = 0;
            for (k = 0; k < ROWS; ++k) {
                if (!row_prime[k]) continue;
                if (block_ok((int)i, (int)j, (int)k))
                    ++deg;
            }
            state_deg[state] = deg;
            total_deg += deg;
        }
    }

    /* compress transition list into a contiguous array */
    trans_start[0] = 0;
    for (i = 1; i <= STATES; ++i)
        trans_start[i] = trans_start[i - 1] + state_deg[i - 1];

    int *pos = (int*)malloc(STATES * sizeof(int));
    memcpy(pos, trans_start, STATES * sizeof(int));

    for (i = 0; i < ROWS; ++i) {
        if (!row_prime[i]) continue;
        for (j = 0; j < ROWS; ++j) {
            if (!row_prime[j]) continue;
            int state = (int)(i * ROWS + j);
            for (k = 0; k < ROWS; ++k) {
                if (!row_prime[k]) continue;
                if (block_ok((int)i, (int)j, (int)k)) {
                    int dst = (int)(j * ROWS + k);
                    trans_list[pos[state]++] = dst;
                }
            }
        }
    }
    free(pos);
    free(state_deg);

    /* allocate DP vectors */
    dp_cur  = (seq_t*)calloc(STATES, sizeof(seq_t));
    dp_next = (seq_t*)calloc(STATES, sizeof(seq_t));

    /* initialise for n = 1 (three rows) */
    for (i = 0; i < ROWS; ++i) {
        if (!row_prime[i]) continue;
        for (j = 0; j < ROWS; ++j) {
            if (!row_prime[j]) continue;
            for (k = 0; k < ROWS; ++k) {
                if (!row_prime[k]) continue;
                if (block_ok((int)i, (int)j, (int)k)) {
                    int idx = (int)(j * ROWS + k);
                    dp_cur[idx] += (seq_t)1;
                }
            }
        }
    }

    /* set up cache for a(n) */
    a_len = 2;                     /* indices 0 and 1 at minimum */
    a_values = (seq_t*)calloc(a_len + 1, sizeof(seq_t));

    /* compute a(1) */
    seq_t sum = 0;
    for (i = 0; i < STATES; ++i)
        sum += dp_cur[i];
    a_values[1] = sum;
    max_n_computed = 1;
    initialized = true;
}

/* ------------------------------------------------------------------------- */
static void step(void)
{
    memset(dp_next, 0, STATES * sizeof(seq_t));
    for (int s = 0; s < STATES; ++s) {
        seq_t cnt = dp_cur[s];
        if (cnt == (seq_t)0) continue;
        int start = trans_start[s];
        int end   = trans_start[s + 1];
        for (int p = start; p < end; ++p) {
            int dst = trans_list[p];
            dp_next[dst] += cnt;
        }
    }
    /* swap current and next vectors */
    seq_t *tmp = dp_cur;
    dp_cur = dp_next;
    dp_next = tmp;
}

/* ------------------------------------------------------------------------- */
static void ensure_a_capacity(idx_t n)
{
    if (n + 1 > a_len) {
        size_t new_len = (n + 1) * 2;
        seq_t *new_arr = (seq_t*)calloc(new_len + 1, sizeof(seq_t));
        if (new_arr) {
            memcpy(new_arr, a_values, (a_len + 1) * sizeof(seq_t));
            free(a_values);
            a_values = new_arr;
            a_len = new_len;
        }
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n < 1)
        return (seq_t)0;

    if (n > max_n_computed) {
        for (idx_t cur = max_n_computed + 1; cur <= n; ++cur) {
            step(); /* add one more row */
            /* sum all states to get a(cur) */
            seq_t total = 0;
            for (int s = 0; s < STATES; ++s)
                total += dp_cur[s];
            ensure_a_capacity(cur);
            a_values[cur] = total;
        }
        max_n_computed = n;
    }

    return a_values[n];
}
