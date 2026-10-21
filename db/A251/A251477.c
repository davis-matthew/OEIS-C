/*
 * A251477  Number of (n+1)X(3+1) 0..3 arrays with every 2X2 subblock summing
 *          to a prime and those sums nondecreasing in every row and column.
 * https://oeis.org/A251477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this sequence is calculated with a dynamic programming over
 *            rows. Each row is represented by a base‑4 encoding of length 4.
 *            The DP state keeps the previous row and the vector of the 2×2
 *            sub‑block sums of the last two rows. Transitions are allowed
 *            when the three sums are primes and non‑decreasing,...
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 26, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Every row is a 4‑digit base‑4 number: 0..255. */
#define DP_ROWS 256
#define DP_COLS 35
#define DP_SIZE (DP_ROWS * DP_COLS)

/* Representation of a row in 4 digits (0..3). */
static unsigned char row_digits[DP_ROWS][4];

/* Lookup whether a sum 0..12 is prime. */
static bool is_prime[13] = {
    false, false, true,  true,  false,
    true,  false, true,  false, false,
    false, true,  false
};

/* Primes that may appear as a 2×2 sum. */
static const unsigned char PRIMES[5] = {2,3,5,7,11};

/* All non‑decreasing triples of the above primes (35 total). */
static unsigned char s_values[DP_COLS][3];
static uint8_t idx_map[13][13][13];  /* 255 = invalid */

/* Information for a pair of rows: whether it is allowed and the
   corresponding sum vector. */
typedef struct {
    bool        valid;
    unsigned char s_idx;   /* index into s_values */
    unsigned char s0,s1,s2;
} PairInfo;
static PairInfo pair_info[DP_ROWS][DP_ROWS];

/* Transition information from a previous row to a next row. */
typedef struct {
    unsigned char next;    /* next row index */
    unsigned char s_idx;   /* sum vector of the pair (pre,next) */
    unsigned char s0,s1,s2;
} NextInfo;
static NextInfo  nextInfos[DP_ROWS * DP_ROWS];
static size_t    next_start[DP_ROWS]; /* starting index in nextInfos per pre row */
static size_t    next_count[DP_ROWS]; /* number of transitions per pre row */

/* DP arrays */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
static bool init_done = false;

/* ------------------------------------------------------------------ */
/* Initialise lookup tables and DP buffers.  Executed once. */
static void initialise(void)
{
    if (init_done) return;
    init_done = true;

    /* Build row_digits. */
    for (unsigned int r = 0; r < DP_ROWS; ++r) {
        unsigned int x = r;
        for (int d = 3; d >= 0; --d) {
            row_digits[r][d] = (unsigned char)(x & 3);
            x >>= 2;
        }
    }

    /* Initialise idx_map to 255 (invalid). */
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            for (int k = 0; k < 13; ++k)
                idx_map[i][j][k] = 255;

    /* Enumerate all non‑decreasing triples of primes. */
    size_t vidx = 0;
    for (int ii = 0; ii < 5; ++ii)
        for (int jj = ii; jj < 5; ++jj)
            for (int kk = jj; kk < 5; ++kk) {
                unsigned char p0 = PRIMES[ii];
                unsigned char p1 = PRIMES[jj];
                unsigned char p2 = PRIMES[kk];
                idx_map[p0][p1][p2] = (uint8_t)vidx;
                s_values[vidx][0] = p0;
                s_values[vidx][1] = p1;
                s_values[vidx][2] = p2;
                ++vidx;
            }

    /* Build pair_info and transition lists. */
    size_t total_next = 0;
    for (size_t pre = 0; pre < DP_ROWS; ++pre) {
        next_start[pre] = total_next;
        for (size_t nxt = 0; nxt < DP_ROWS; ++nxt) {
            unsigned char *pd_pre = row_digits[pre];
            unsigned char *pd_nxt = row_digits[nxt];
            unsigned char sum0 = pd_pre[0] + pd_pre[1] + pd_nxt[0] + pd_nxt[1];
            unsigned char sum1 = pd_pre[1] + pd_pre[2] + pd_nxt[1] + pd_nxt[2];
            unsigned char sum2 = pd_pre[2] + pd_pre[3] + pd_nxt[2] + pd_nxt[3];
            bool ok = is_prime[sum0] && is_prime[sum1] && is_prime[sum2]
                      && sum0 <= sum1 && sum1 <= sum2;
            if (ok) {
                uint8_t sidx = idx_map[sum0][sum1][sum2];
                pair_info[pre][nxt] = (PairInfo){true, sidx, sum0,sum1,sum2};
                nextInfos[total_next] = (NextInfo){(unsigned char)nxt, sidx,
                                                   sum0,sum1,sum2};
                ++total_next;
            } else {
                pair_info[pre][nxt] = (PairInfo){false, 255, 0,0,0};
            }
        }
        next_count[pre] = total_next - next_start[pre];
    }

    /* Allocate DP buffers. */
    dp_cur   = (seq_t*)malloc(DP_SIZE * sizeof(seq_t));
    dp_next  = (seq_t*)malloc(DP_SIZE * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Works for any n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for offset <1, return 0 */

    initialise();

    /* Clear current DP. */
    for (size_t i = 0; i < DP_SIZE; ++i) dp_cur[i] = (seq_t)0;

    /* Initialise from the first pair of rows. */
    for (size_t r0 = 0; r0 < DP_ROWS; ++r0) {
        for (size_t r1 = 0; r1 < DP_ROWS; ++r1) {
            if (!pair_info[r0][r1].valid) continue;
            size_t state = r1 * DP_COLS + pair_info[r0][r1].s_idx;
            dp_cur[state] += (seq_t)1;
        }
    }

    /* For n == 1 we already counted all valid 2‑row arrays. */
    if (n == 1) {
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < DP_SIZE; ++i) total += dp_cur[i];
        return total;
    }

    /* DP for remaining steps (adding rows). */
    for (idx_t step = 1; step < n; ++step) {
        /* Reset dp_next. */
        for (size_t i = 0; i < DP_SIZE; ++i) dp_next[i] = (seq_t)0;

        for (size_t state = 0; state < DP_SIZE; ++state) {
            seq_t cnt = dp_cur[state];
            if (cnt == (seq_t)0) continue;

            unsigned char prev_row = (unsigned char)(state / DP_COLS);
            unsigned char prev_s   = (unsigned char)(state % DP_COLS);
            unsigned char *prev_vals = s_values[prev_s];

            size_t start = next_start[prev_row];
            size_t end   = start + next_count[prev_row];
            for (size_t idx = start; idx < end; ++idx) {
                const NextInfo *ti = &nextInfos[idx];
                /* Enforce column‑wise monotonicity across rows. */
                if (ti->s0 < prev_vals[0] ||
                    ti->s1 < prev_vals[1] ||
                    ti->s2 < prev_vals[2]) continue;

                size_t new_state = ((size_t)ti->next) * DP_COLS + ti->s_idx;
                dp_next[new_state] += cnt;
            }
        }
        /* Swap DP buffers. */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }

    /* Sum all counts in the final DP state. */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < DP_SIZE; ++i) total += dp_cur[i];
    return total;
}
