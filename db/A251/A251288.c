/*
 * A251288  Number of (n+1)X(4+1) 0..1 arrays with every 2X2 subblock having a
 *          single 1 or two 1s on the same edge or main diagonally.
 * https://oeis.org/A251288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251288: Number of (n+1)×5 0/1 arrays in which every 2×2 subblock
    contains exactly one 1 or exactly two 1s that are either
    on a common edge or on the main diagonal of the subblock.
    The implementation uses dynamic programming over the 32 possible
    5‑bit row patterns.  No closed form or simple recurrence is known
    for this sequence; the algorithm runs in O(32²·n) time and uses
    O(32) memory for the DP state and a small memoisation array.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>             /* for size_t */

/*  Compatibility matrix: comp[p][c] is true iff rows p and c
    may stand next to each other.  */
static bool comp[32][32];

/*  DP arrays – at most 32 states.  */
static seq_t state_cur[32];
static seq_t state_next[32];

/*  Pointers used to swap the current/next arrays.  */
static seq_t *cur_ptr  = state_cur;
static seq_t *next_ptr = state_next;

/*  Largest n that has already been computed (in the memoisation
    array).  */
static idx_t max_n_computed = 0;

/*  Flag indicating whether tables have been initialised.  */
static bool init_done = false;

/*  Memoisation array for results.  For C++ the vector grows automatically,
    for C it is resized with realloc.  */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> results;
#else
static seq_t *results = NULL;
static size_t results_cap = 0;
#endif

/*  Initialise everything once.  */
static void init_tables(void)
{
    /*  Pre‑compute the compatibility of each pair of rows.  */
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int a = (prev >> j) & 1;            /* top left   */
                int b = (prev >> (j + 1)) & 1;      /* top right  */
                int c = (curr >> j) & 1;            /* bottom left */
                int d = (curr >> (j + 1)) & 1;      /* bottom right */
                int ones = a + b + c + d;
                if (ones != 1 && ones != 2) {
                    ok = false;
                    break;
                }
                if (ones == 2) {
                    /*  Reject the anti‑diagonal pattern (b=1,c=1,a=0,d=0).  */
                    if (a == 0 && b == 1 && c == 1 && d == 0) {
                        ok = false;
                        break;
                    }
                }
            }
            comp[prev][curr] = ok;
        }
    }

    /*  Every 5‑bit pattern is allowed as the first row.  */
    for (int s = 0; s < 32; ++s) {
        cur_ptr[s] = (seq_t)1;
    }

    /*  Initialise the results array.  */
#ifdef __cplusplus
    results.resize(2);
    results[0] = (seq_t)0;
    results[1] = (seq_t)0;
#else
    results_cap = 2;
    results = calloc(results_cap, sizeof(seq_t));
    results[0] = (seq_t)0;
    results[1] = (seq_t)0;
#endif

    max_n_computed = 0;
    init_done = true;
}

/*  Ensure that the results array can hold indices up to n.  */
static void ensure_results_capacity(idx_t n)
{
#ifdef __cplusplus
    if (results.size() <= (size_t)n) {
        results.resize((size_t)n + 1);
    }
#else
    while (results_cap <= (size_t)n) {
        size_t new_cap = results_cap * 2;
        if (new_cap < 2) new_cap = 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) abort();         /* out of memory */
        results = tmp;
        results_cap = new_cap;
    }
#endif
}

/*  Public sequence function required by the harness.  */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* sequence is defined for n >= 1 */

    if (!init_done) init_tables();

    /*  Ensure the memoisation array is large enough.  */
    ensure_results_capacity(n);

    /*  Compute DP up to n if necessary.  */
    for (idx_t k = max_n_computed + 1; k <= n; ++k) {
        /*  Clear the next state array.  */
        for (int s = 0; s < 32; ++s) next_ptr[s] = (seq_t)0;

        /*  Propagate transitions from the previous row.  */
        for (int prev = 0; prev < 32; ++prev) {
            seq_t ways = cur_ptr[prev];
            if (ways == (seq_t)0) continue;
            for (int curr = 0; curr < 32; ++curr) {
                if (comp[prev][curr]) {
                    next_ptr[curr] += ways;
                }
            }
        }

        /*  Sum over all ending rows to get the total number
            of (k+1)×5 arrays.  */
        seq_t total = (seq_t)0;
        for (int s = 0; s < 32; ++s) total += next_ptr[s];

        results[k] = total;

        /*  Swap the state buffers for the next iteration.  */
        seq_t *tmp = cur_ptr;
        cur_ptr = next_ptr;
        next_ptr = tmp;
    }

    max_n_computed = n;
    return results[n];
}
