/*
 * A250938  Number of (n+1)X(4+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 29, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   OEIS A250938 – Number of (n+1)×5 arrays with entries in {0,1,2}
   satisfying:
     1. nondecreasing x(i,j)+x(i,j‑1) in the i direction
     2. nondecreasing |x(i,j)-x(i‑1,j)| in the j direction

   The sequence is obtained as the number of walks of length n in a
   directed graph whose vertices are all 3^5 possible rows.  An edge
   i → j exists iff the two rows satisfy the two above constraints
   for every column pair.  The count a(n) equals 1ᵀ Aⁿ 1 where A is the
   adjacency matrix.  For each n ≥ 1 this program returns a(n).

*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* 3^5 = 243 possible rows, constant at compile time */
#define NODES 243

/* adjacency list storage */
static uint16_t adj_list[NODES][NODES];   /* NODES ≤ 255, fits in uint16_t */
static uint8_t  adj_deg[NODES];

/* decoded digits of each row (base‑3 representation) */
static uint8_t row_digits[NODES][5];

/* DP arrays for walks of current and next length */
static seq_t cur_dp[NODES];
static seq_t next_dp[NODES];

/* cache of computed sequence values */
static seq_t *a_cache = NULL;     /* array of a(0..max_n) */
static size_t a_cache_size = 0;   /* allocated capacity */
static idx_t  cur_n_max = 0;      /* maximum n for which a_cache[] is filled */

/* flag whether initialization has been performed */
static bool initialized = false;

/* ------------------------------------------------------------ */
/*  Build adjacency lists and initialise starting DP          */
/* ------------------------------------------------------------ */
static void build_adjacency(void)
{
    /* decode all rows into base‑3 digits */
    for (idx_t r = 0; r < (idx_t)NODES; ++r) {
        idx_t tmp = r;
        for (int pos = 4; pos >= 0; --pos) {
            row_digits[r][pos] = (uint8_t)(tmp % 3u);
            tmp /= 3u;
        }
    }

    /* compute adjacency between rows */
    for (idx_t i = 0; i < (idx_t)NODES; ++i) {
        const uint8_t *a = row_digits[i];
        for (idx_t j = 0; j < (idx_t)NODES; ++j) {
            const uint8_t *b = row_digits[j];
            bool ok = true;

            /* sum constraint in i direction */
            for (int col = 1; col <= 4 && ok; ++col) {
                int s_a = a[col] + a[col - 1];
                int s_b = b[col] + b[col - 1];
                if (s_a > s_b) ok = false;
            }

            /* difference constraint in j direction */
            for (int col = 0; col <= 3 && ok; ++col) {
                int da = (int) b[col] - (int) a[col];
                if (da < 0) da = -da;
                int db = (int) b[col + 1] - (int) a[col + 1];
                if (db < 0) db = -db;
                if (da > db) ok = false;
            }

            if (ok) {
                adj_list[i][adj_deg[i]++] = (uint16_t)j;
            }
        }
    }

    /* initial DP state: walks of length 0 (single row) */
    for (int i = 0; i < NODES; ++i) cur_dp[i] = 1;

    /* allocate cache for a(0) */
    a_cache = (seq_t *)malloc(sizeof(seq_t));
    if (!a_cache) abort();
    a_cache[0] = (seq_t)NODES;   /* 3^5 */
    a_cache_size = 1;
    cur_n_max = 0;
}

/* ------------------------------------------------------------ */
/*  Ensure that a_cache can hold at least the index 'idx'      */
/* ------------------------------------------------------------ */
static void ensure_cache_capacity(idx_t idx)
{
    if ((size_t)idx < a_cache_size) return;
    size_t new_size = a_cache_size ? a_cache_size * 2 : 1;
    while (new_size <= (size_t)idx) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* memory exhausted */
    a_cache = tmp;
    a_cache_size = new_size;
}

/* ------------------------------------------------------------ */
/*  Extend the DP to reach at least 'n' steps (walk length n)   */
/* ------------------------------------------------------------ */
static void extend_to_n(idx_t n)
{
    for (idx_t step = cur_n_max + 1; step <= n; ++step) {
        /* zero next DP array */
        memset(next_dp, 0, sizeof(next_dp));

        /* propagate transitions from current state */
        for (int i = 0; i < NODES; ++i) {
            seq_t val = cur_dp[i];
            if (!val) continue;
            uint8_t deg = adj_deg[i];
            const uint16_t *neigh = adj_list[i];
            for (uint8_t k = 0; k < deg; ++k) {
                next_dp[neigh[k]] += val;
            }
        }

        /* compute total number of walks of this length */
        seq_t total = 0;
        for (int j = 0; j < NODES; ++j) total += next_dp[j];

        /* store result in cache */
        ensure_cache_capacity(step);
        a_cache[step] = total;

        /* prepare for next iteration */
        memcpy(cur_dp, next_dp, sizeof(cur_dp));
    }
    cur_n_max = n;
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_adjacency();
        initialized = true;
    }

    if (n > cur_n_max) extend_to_n(n);
    return a_cache[n];
}
