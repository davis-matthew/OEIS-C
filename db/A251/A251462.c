/*
 * A251462  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock summing
 *          to a prime and those sums nondecreasing in every row and column.
 * https://oeis.org/A251462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 37, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming:
 * a(n) = number of (n+1)×4 arrays with entries 0..2 such that
 * each 2×2 subblock sums to a prime and the resulting 2×2 sums are
 * nondecreasing left→right in every row and top→bottom in every
 * column.  The DP enumerates all admissible pairs of consecutive rows
 * (states), then walks n-1 transitions.  The initial states are all
 * valid pairs of rows; each transition appends a new row while keeping
 * the monotonic and prime constraints.  The result is the total number
 * of paths of length n-1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Doubly linked list of integers (used for state indexing) */
typedef struct {
    int *data;
    size_t cnt;
    size_t cap;
} IntList;

static void
intlist_push(IntList *list, int val)
{
    if (list->cnt == list->cap) {
        size_t newcap = list->cap ? list->cap * 2 : 4;
        int *newdata = (int *)realloc(list->data, newcap * sizeof(int));
        list->data = newdata;
        list->cap = newcap;
    }
    list->data[list->cnt++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static data that is initialised only once. */
    static bool initialized = false;
    static size_t S;                     /* number of admissible row‑pairs (states) */
    static int *state_prev;               /* first row of a state (index 0..80) */
    static int *state_curr;               /* second row of a state (index 0..80) */
    static int (*state_sums)[3];          /* 2×2 sums for a state */
    static IntList *row_start_lists;      /* states starting with a given row */
    static IntList *adjacency;            /* adjacency lists of states (vertical) */
    static seq_t *dp_cur = NULL;          /* DP array for current step */
    static seq_t *dp_next = NULL;         /* DP array for next step */

    /* Build all necessary data on first call. */
    if (!initialized) {
        /* Precompute the 81 possible 4‑digit rows in base 3. */
        int row_vals[81][4];
        for (int v = 0; v < 81; ++v) {
            row_vals[v][0] = (v / 27) % 3;
            row_vals[v][1] = (v / 9) % 3;
            row_vals[v][2] = (v / 3) % 3;
            row_vals[v][3] = v % 3;
        }

        /* Prime flag for sums 0..8. */
        static const char prime_flag[9] = {
            0, 0, /* 0,1 */
            1, 1, /* 2,3 */
            0,     /* 4 */
            1,     /* 5 */
            0,     /* 6 */
            1,     /* 7 */
            0      /* 8 */
        };

        /* Allocate temporary storage for all possible states (81×81). */
        size_t max_states = 81 * 81;
        state_prev = (int *)malloc(max_states * sizeof(int));
        state_curr = (int *)malloc(max_states * sizeof(int));
        state_sums = (int (*)[3])malloc(max_states * sizeof(int[3]));
        row_start_lists = (IntList *)malloc(81 * sizeof(IntList));
        for (int i = 0; i < 81; ++i) {
            row_start_lists[i].data = NULL;
            row_start_lists[i].cnt = 0;
            row_start_lists[i].cap = 0;
        }

        /* Enumerate all valid states. */
        S = 0;
        for (int p = 0; p < 81; ++p) {
            for (int q = 0; q < 81; ++q) {
                int s[3];
                s[0] = row_vals[p][0] + row_vals[p][1] +
                       row_vals[q][0] + row_vals[q][1];
                s[1] = row_vals[p][1] + row_vals[p][2] +
                       row_vals[q][1] + row_vals[q][2];
                s[2] = row_vals[p][2] + row_vals[p][3] +
                       row_vals[q][2] + row_vals[q][3];
                if (!prime_flag[s[0]] || !prime_flag[s[1]] || !prime_flag[s[2]])
                    continue;
                if (!(s[0] <= s[1] && s[1] <= s[2]))
                    continue;  /* horizontal monotonicity */

                state_prev[S] = p;
                state_curr[S] = q;
                state_sums[S][0] = s[0];
                state_sums[S][1] = s[1];
                state_sums[S][2] = s[2];

                intlist_push(&row_start_lists[p], (int)S);
                ++S;
            }
        }

        /* Allocate adjacency lists. */
        adjacency = (IntList *)malloc(S * sizeof(IntList));
        for (size_t i = 0; i < S; ++i) {
            adjacency[i].data = NULL;
            adjacency[i].cnt = 0;
            adjacency[i].cap = 0;
        }

        /* Build directed graph edges: state i -> state j if
         * state_curr[i] == state_prev[j] and sums are nondecreasing vertically. */
        for (size_t i = 0; i < S; ++i) {
            int mid = state_curr[i];
            IntList *list = &row_start_lists[mid];
            for (size_t t = 0; t < list->cnt; ++t) {
                int j = list->data[t];
                bool ok = true;
                for (int k = 0; k < 3; ++k)
                    if (state_sums[i][k] > state_sums[j][k]) {
                        ok = false;
                        break;
                    }
                if (ok)
                    intlist_push(&adjacency[i], j);
            }
        }

        /* Allocate DP arrays. */
        dp_cur = (seq_t *)malloc(S * sizeof(seq_t));
        dp_next = (seq_t *)malloc(S * sizeof(seq_t));
        initialized = true;
    }

    /* For each call, reinitialise dp_cur to 1 (fresh starts). */
    for (size_t i = 0; i < S; ++i)
        dp_cur[i] = (seq_t)1;
    for (size_t i = 0; i < S; ++i)
        dp_next[i] = (seq_t)0;

    /* n == 1 corresponds to exactly two rows. */
    if (n == 1)
        return (seq_t)S;

    /* DP for n-1 transitions (each adds one more row). */
    for (idx_t step = 1; step < n; ++step) {
        for (size_t i = 0; i < S; ++i)
            dp_next[i] = (seq_t)0;

        for (size_t i = 0; i < S; ++i) {
            seq_t cur = dp_cur[i];
            if (cur == (seq_t)0)
                continue;
            IntList *adj = &adjacency[i];
            for (size_t t = 0; t < adj->cnt; ++t) {
                int j = adj->data[t];
                dp_next[j] += cur;
            }
        }
        /* Swap dp arrays for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all terminal states. */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < S; ++i)
        ans += dp_cur[i];
    return ans;
}
