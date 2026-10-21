/*
 * A250464  Number of (n+1) X (4+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250464: Number of (n+1)×5 binary arrays satisfying
   the two monotonicity constraints.  The implementation uses
   dynamic programming over the 32 possible row masks.  The sequence
   values are memoised so that arbitrary order of calls is O(1)
   per new n.  */
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Transition table: trans[prev][cur] == true iff a row 'cur' may
   follow a row 'prev' while satisfying both monotonicity constraints.  */
static bool trans[32][32] = {0};
static bool trans_ready = false;

/* DP state arrays; 32 rows possible, indexed by bitmask 0..31.      */
static seq_t dp_cur[32];
static seq_t dp_next[32];
static bool dp_ready = false;

/* Cache of computed sequence values: a_cache[i] == a(i).           */
static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;           /* length of allocated cache   */
static idx_t current_n = 0;             /* largest n for which a_cache known */

/* ------------------------------------------------------------------ */
/* Build the 32×32 transition table.                                   */
static void build_transition(void)
{
    for (int P = 0; P < 32; ++P) {
        for (int C = 0; C < 32; ++C) {
            bool ok = true;

            /* Constraint 1: vertical monotonicity of min(x(i,j),x(i,j-1)). */
            for (int j = 1; j <= 4; ++j) {
                bool prev_min = ((P >> (j-1)) & 1) && ((P >> j) & 1);
                bool curr_min = ((C >> (j-1)) & 1) && ((C >> j) & 1);
                if (prev_min && !curr_min) {   /* would decrease */
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                trans[P][C] = false;
                continue;
            }

            /* Constraint 2: horizontal monotonicity of min(x(i,j),x(i-1,j)). */
            bool last_vmin = ((P >> 0) & 1) && ((C >> 0) & 1);
            for (int j = 1; j <= 4; ++j) {
                bool curr_vmin = ((P >> j) & 1) && ((C >> j) & 1);
                if (curr_vmin < last_vmin) {   /* would decrease */
                    ok = false;
                    break;
                }
                last_vmin = curr_vmin;
            }
            trans[P][C] = ok;
        }
    }
    trans_ready = true;
}

/* ------------------------------------------------------------------ */
/* Ensure that a_cache[1..target_n] are computed and stored.        */
static void ensure_up_to(idx_t target_n)
{
    if (target_n <= current_n)
        return;

    if (!trans_ready)
        build_transition();

    /* Resize cache if needed. */
    if (a_cache == NULL || a_cache_len < target_n + 1) {
        size_t new_len = (size_t)(target_n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();                 /* out of memory; terminate */
        a_cache = tmp;
        for (size_t i = a_cache_len; i < new_len; ++i)
            a_cache[i] = (seq_t)0;   /* initialise new entries to zero */
        a_cache_len = new_len;
    }

    /* Initialise DP on first use. */
    if (!dp_ready) {
        for (int m = 0; m < 32; ++m)
            dp_cur[m] = (seq_t)1;    /* all 32 masks are possible initially */
        dp_ready = true;
        a_cache[0] = (seq_t)0;       /* unused but set for safety */
        current_n = 0;
    }

    /* Run DP steps to reach target_n. */
    for (idx_t step = current_n + 1; step <= target_n; ++step) {
        for (int m = 0; m < 32; ++m)
            dp_next[m] = (seq_t)0;

        for (int P = 0; P < 32; ++P) {
            if (dp_cur[P] == (seq_t)0)
                continue;
            for (int C = 0; C < 32; ++C) {
                if (trans[P][C]) {
                    dp_next[C] += dp_cur[P];
                }
            }
        }

        seq_t sum = (seq_t)0;
        for (int m = 0; m < 32; ++m)
            sum += dp_next[m];
        a_cache[step] = sum;

        /* Swap dp_cur and dp_next for next iteration. */
        for (int m = 0; m < 32; ++m) {
            seq_t tmp_buf = dp_cur[m];
            dp_cur[m] = dp_next[m];
            dp_next[m] = tmp_buf;
        }
    }
    current_n = target_n;
}

/* ------------------------------------------------------------------ */
/* Public entry point for the sequence.                               */
seq_t a(idx_t n)
{
    if (n == 0)           /* not used in OEIS offset, but safe */
        return (seq_t)0;
    ensure_up_to(n);
    return a_cache[n];
}
