/*
 * A250630  Number of (n+1)X(6+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 60, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: A250630 – number of (n+1)×7 arrays over {0,1,2}
 * with the constraints  
 *   (1) for all j≥1,  x(i,j)+x(i,j-1) is nondecreasing as i increases,
 *   (2) for all j≥1,  min(x(i,j),x(i-1,j)) is nondecreasing as j increases.  
 *
 * The algorithm constructs the graph of all 3^7 = 2187 possible row
 * patterns and precomputes all legal transitions between them.
 * A dynamic programming (DP) vector is kept over this graph.
 * dp_cur[s] = number of arrays with the current height that end in row
 * pattern s.  Starting from one row, a new row is added by multiplying
 * the DP vector with the adjacency matrix.  After each multiplication
 * the total sum of the vector is stored in a_cache[n], giving the
 * required value a(n) (n≥1 corresponds to arrays with n+1 rows).  The
 * routine is memoised, so arbitrary call order and n up to about 10000
 * are handled in well under 2 s. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define STATES 2187   /* 3^7 */

typedef struct {
    unsigned short *next;   /* array of destination state indices */
    int count;              /* actual number of neighbours */
    int capacity;           /* allocated capacity of 'next' */
} Adj;

/* DP storage */
static seq_t dp_a[STATES];
static seq_t dp_b[STATES];
static seq_t *dp_cur = dp_a;   /* pointer to current DP row */
static seq_t *dp_next = dp_b;  /* pointer to next DP row */

/* Graph and auxiliary data */
static Adj adjacency[STATES];
static uint8_t row_vec[STATES][7];

/* Memoisation of a(n) */
static idx_t computed_steps = 0;      /* largest n already computed */
static seq_t *a_cache = NULL;         /* cache array (index 1 … ) */
static size_t a_cache_cap = 0;        /* capacity of a_cache */

/* Flag whether the global data have been initialised */
static int initialized = 0;

/* --------------------------------------------------------------------- */
/* Helper: extend adjacency list of state 'from' with state 'to'.        */
static void push_adj(int from, unsigned short to)
{
    Adj *a = &adjacency[from];
    if (a->count == a->capacity) {
        a->capacity = a->capacity ? a->capacity * 2 : 4;
        a->next = (unsigned short *)realloc(a->next,
                                            a->capacity * sizeof(unsigned short));
    }
    a->next[a->count++] = to;
}

/* --------------------------------------------------------------------- */
/* Initialise all global data: generate row vectors, build adjacency,    */
static void init(void)
{
    /* Pre‑compute base‑3 representation of all 2187 states */
    for (int code = 0; code < STATES; ++code) {
        int tmp = code;
        for (int j = 0; j < 7; ++j) {
            row_vec[code][j] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
    }

    /* Build adjacency lists */
    for (int p = 0; p < STATES; ++p) {
        adjacency[p].next = NULL;
        adjacency[p].count = 0;
        adjacency[p].capacity = 0;
        for (int s = 0; s < STATES; ++s) {
            bool ok = true;

            /* Condition 1: sums of adjacent columns are nondecreasing */
            for (int j = 1; j < 7 && ok; ++j) {
                int sum_prev = row_vec[p][j] + row_vec[p][j - 1];
                int sum_cur  = row_vec[s][j] + row_vec[s][j - 1];
                if (sum_cur < sum_prev) ok = false;
            }
            if (!ok) continue;

            /* Condition 2: minimums over columns are nondecreasing */
            unsigned short mins[7];
            for (int j = 0; j < 7; ++j)
                mins[j] = row_vec[s][j] < row_vec[p][j] ? row_vec[s][j] : row_vec[p][j];
            for (int j = 0; j < 6 && ok; ++j)
                if (mins[j] > mins[j + 1]) ok = false;
            if (!ok) continue;

            push_adj(p, (unsigned short)s);
        }
    }

    /* DP for one row: exactly one array of height 1 for each state */
    for (int i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)1;

    /* Initialise cache */
    a_cache_cap = 1024;
    a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
    a_cache[0] = (seq_t)0;   /* dummy entry for index 0 */
    computed_steps = 0;
    initialized = 1;
}

/* --------------------------------------------------------------------- */
/* Compute DP layers until we reach at least n (indexing from 1).          */
static void ensure_computed(idx_t n)
{
    while (computed_steps < n) {
        /* zero next layer */
        for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

        /* transition */
        for (int p = 0; p < STATES; ++p) {
            seq_t val = dp_cur[p];
            if (val == (seq_t)0) continue;
            const Adj *a = &adjacency[p];
            for (int k = 0; k < a->count; ++k) {
                unsigned short s = a->next[k];
                dp_next[s] += val;
            }
        }

        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        ++computed_steps;

        /* sum over all states to get a(computed_steps) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < STATES; ++i) sum += dp_cur[i];

        /* grow cache if necessary */
        if ((size_t)computed_steps >= a_cache_cap) {
            a_cache_cap *= 2;
            a_cache = (seq_t *)realloc(a_cache,
                                       a_cache_cap * sizeof(seq_t));
        }
        a_cache[computed_steps] = sum;
    }
}

/* --------------------------------------------------------------------- */
/* Public interface */
seq_t a(idx_t n)
{
    if (!initialized) init();
    if (n < 1) return (seq_t)0;          /* defined only for n ≥ 1 */
    ensure_computed(n);
    return a_cache[n];
}
