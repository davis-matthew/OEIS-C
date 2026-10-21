/*
 * A262468  Number of (n+1)X(4+1) 0..1 arrays with each row divisible by 3 and
 *          each column divisible by 5, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A262468
 *
 *  a(n) = number of (n+1)×5 binary arrays whose rows (viewed as
 *  5‑bit numbers) are divisible by 3 and whose columns (viewed as
 *  (n+1)-bit numbers) are divisible by 5.  The values are computed
 *  using a deterministic automaton DP over column residues modulo 5.
 *  Complexity: O((n+1)·3125·11) per call.  The function is memoised
 *  to be efficient for repeated, non‑monotonic calls.  It works for
 *  any build that supplies `seq_t` and `idx_t`.
 */

#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------------
 * Helper: encode a vector of 5 residues (0..4) into a state index
 * in base‑5 representation (0..3124).
 * ---------------------------------------------------------------------- */
static inline unsigned encode_state(const unsigned r[5])
{
    return r[0] + r[1] * 5 + r[2] * 25 + r[3] * 125 + r[4] * 625;
}

/* ----------------------------------------------------------------------
 *  Pre‑computed list of 5‑bit patterns that are ≡0 (mod 3).
 *  There are 11 such patterns: 00000 (0), 00011 (3), 00110 (6),
 *  …, 11110 (30).
 * ---------------------------------------------------------------------- */
static const unsigned int valid_rows[11] = {
    0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30
};

/* ----------------------------------------------------------------------
 *  Transition table: next_state[old_state][row_pattern_index]
 *  -> new_state.
 *  Size: 3125 × 11
 * ---------------------------------------------------------------------- */
static unsigned next_state[3125][11];
static int trans_inited = 0;

/* ----------------------------------------------------------------------
 *  Initialise transition table once.
 * ---------------------------------------------------------------------- */
static void init_transitions(void)
{
    if (trans_inited) return;
    for (unsigned s = 0; s < 3125; ++s) {
        unsigned r[5];
        unsigned tmp = s;
        for (int j = 0; j < 5; ++j) {
            r[j] = tmp % 5;
            tmp /= 5;
        }
        for (int p = 0; p < 11; ++p) {
            unsigned v = valid_rows[p];
            unsigned r2[5];
            for (int j = 0; j < 5; ++j) {
                unsigned bit = (v >> (4 - j)) & 1u;
                r2[j] = (r[j] * 2 + bit) % 5;
            }
            next_state[s][p] = encode_state(r2);
        }
    }
    trans_inited = 1;
}

/* ----------------------------------------------------------------------
 *  Memoised state for incremental computation.
 * ---------------------------------------------------------------------- */
static idx_t memo_max_n   = 0;          /* largest n already cached (>=0) */
static seq_t *memo_cache  = NULL;       /* memo_cache[n] = a(n) for n ≥ 1 */
static size_t memo_cap    = 0;           /* capacity of memo_cache */

static seq_t *dp_cur = NULL, *dp_next = NULL; /* DP arrays of length 3125 */
static idx_t rows_done = 0;                 /* number of rows processed so far */

/* ----------------------------------------------------------------------
 *  Ensure memo_cache has capacity for at least n+1 entries (n ≥ 0).
 * ---------------------------------------------------------------------- */
static void ensure_cache_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (memo_cache == NULL || need > memo_cap) {
        seq_t *new_cache = (seq_t*)calloc(need, sizeof(seq_t));
        if (memo_cache != NULL) {
            memcpy(new_cache, memo_cache,
                   (size_t)(memo_max_n + 1) * sizeof(seq_t));
            free(memo_cache);
        }
        memo_cache = new_cache;
        memo_cap   = need;
    }
}

/* ----------------------------------------------------------------------
 *  Ensure DP arrays are allocated.
 * ---------------------------------------------------------------------- */
static void ensure_dp_arrays(void)
{
    if (dp_cur == NULL) {
        dp_cur = (seq_t*)calloc(3125, sizeof(seq_t));
        dp_next = (seq_t*)calloc(3125, sizeof(seq_t));
        dp_cur[0] = (seq_t)1;    /* start with all column residues 0 */
        rows_done = 0;
    }
}

/* ----------------------------------------------------------------------
 *  Compute a(n) for all n up to need_n incrementally.
 * ---------------------------------------------------------------------- */
static void compute_up_to(idx_t need_n)
{
    init_transitions();
    ensure_dp_arrays();
    ensure_cache_capacity(need_n);

    for (idx_t target_rows = rows_done + 1; target_rows <= need_n + 1;
         ++target_rows) {

        memset(dp_next, 0, 3125 * sizeof(seq_t));

        for (unsigned s = 0; s < 3125; ++s) {
            seq_t cur_val = dp_cur[s];
            if (cur_val == (seq_t)0) continue;
            for (int p = 0; p < 11; ++p) {
                unsigned ns = next_state[s][p];
                dp_next[ns] += cur_val;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        rows_done = target_rows;

        /* Store result if this corresponds to an n (rows = n+1). */
        if (target_rows >= 2) {
            idx_t n = target_rows - 1;
            memo_cache[n] = dp_cur[0];    /* all column residues must be 0 */
        }
    }

    if (need_n > memo_max_n) memo_max_n = need_n;
}

/* ----------------------------------------------------------------------
 *  Function required by the build system.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1; undefined for n=0 */
    if (memo_max_n < n) compute_up_to(n);
    return memo_cache[n];
}
