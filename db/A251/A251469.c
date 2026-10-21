/*
 * A251469  Number of (n+1)X(2+1) 0..5 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of (n+1)×3 arrays over {0..5}
   with every 2×2 subblock summing to a prime. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

#define ROWS 216   /* 6^3 possible rows */

/* Helper: check if a 2x2 block has prime sum (sum between 0 and 20). */
static inline bool prime_sum(int a, int b, int c, int d)
{
    int s = a + b + c + d;
    /* 0..20 */
    static const bool prime[21] = {
        false, /*0*/
        false, /*1*/
        true,  /*2*/
        true,  /*3*/
        false, /*4*/
        true,  /*5*/
        false, /*6*/
        true,  /*7*/
        false, /*8*/
        false, /*9*/
        false, /*10*/
        true,  /*11*/
        false, /*12*/
        true,  /*13*/
        false, /*14*/
        false, /*15*/
        false, /*16*/
        true,  /*17*/
        false, /*18*/
        true,  /*19*/
        false  /*20*/
    };
    return prime[s];
}

seq_t a(idx_t n)
{
    /*  static state ---------------------------------------------------- */
    static bool built = false;

    /* adjacency lists for the row-state automaton */
    static int **next_list = NULL;     /* next_list[row] -> array of following rows */
    static int *next_deg   = NULL;     /* next_deg[row] -> number of following rows */

    /* DP state vectors */
    static seq_t *dp_cur  = NULL;
    static seq_t *dp_next = NULL;

    /* sequence cache: cache[i] = number of arrays with (i) rows */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;        /* largest length currently cached (>=1) */

    /*  build everything once ------------------------------------------ */
    if (!built)
    {
        /* allocate adjacency structures */
        next_list = (int **)malloc(ROWS * sizeof(int *));
        next_deg  = (int *)malloc(ROWS * sizeof(int));
        if (!next_list || !next_deg) abort();

        /* temporary degrees for counting edges */
        int deg[ROWS] = {0};

        /* Count reachable rows for each r */
        for (int r = 0; r < ROWS; ++r)
        {
            int r0 = r / 36;
            int r1 = (r / 6) % 6;
            int r2 = r % 6;

            for (int s = 0; s < ROWS; ++s)
            {
                int s0 = s / 36;
                int s1 = (s / 6) % 6;
                int s2 = s % 6;

                if (prime_sum(r0, r1, s0, s1) && prime_sum(r1, r2, s1, s2))
                    ++deg[r];
            }
        }

        /* Allocate adjacency lists. */
        for (int r = 0; r < ROWS; ++r)
        {
            if (deg[r] > 0)
            {
                next_list[r] = (int *)malloc(deg[r] * sizeof(int));
                if (!next_list[r]) abort();
            }
            else
                next_list[r] = NULL;
            next_deg[r] = deg[r];
        }

        /* Fill adjacency lists. */
        int idx[ROWS] = {0};
        for (int r = 0; r < ROWS; ++r)
        {
            int r0 = r / 36;
            int r1 = (r / 6) % 6;
            int r2 = r % 6;

            for (int s = 0; s < ROWS; ++s)
            {
                int s0 = s / 36;
                int s1 = (s / 6) % 6;
                int s2 = s % 6;

                if (prime_sum(r0, r1, s0, s1) && prime_sum(r1, r2, s1, s2))
                {
                    next_list[r][idx[r]++] = s;
                }
            }
        }

        /* Allocate DP vectors with proper construction of seq_t objects. */
        dp_cur  = seq_alloc(ROWS);
        dp_next = seq_alloc(ROWS);
        if (!dp_cur || !dp_next) abort();

        /* initialize for length 1 (single row). */
        for (int r = 0; r < ROWS; ++r)
            dp_cur[r] = (seq_t)1;

        /* cache[1] = number of arrays of length 1 */
        cache_cap = 2;                        /* allocate indices 0,1 */
        cache = seq_alloc(cache_cap);
        if (!cache) abort();
        cache[1] = (seq_t)ROWS;
        cache_len = 1;

        built = true;
    }

    /* -------------------------------------------------- compute requested n */
    size_t needed_len = (size_t)n + 1;   /* number of rows for the array */

    /* extend cache up to needed_len */
    while (cache_len < needed_len)
    {
        /* zero next DP vector */
        for (int r = 0; r < ROWS; ++r)
            dp_next[r] = (seq_t)0;

        /* transition from dp_cur to dp_next */
        for (int r = 0; r < ROWS; ++r)
        {
            seq_t val = dp_cur[r];
            if (val == (seq_t)0) continue;

            int d    = next_deg[r];
            int *lst = next_list[r];
            for (int k = 0; k < d; ++k)
                dp_next[lst[k]] = dp_next[lst[k]] + val;
        }

        /* compute total for this length */
        seq_t total = (seq_t)0;
        for (int r = 0; r < ROWS; ++r)
            total = total + dp_next[r];

        /* ensure cache capacity */
        if (cache_len + 1 >= cache_cap)
        {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            while (new_cap <= cache_len + 1)
                new_cap *= 2;

            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len + 1] = total;

        /* swap DP vectors for next iteration */
        seq_t *tmp_dp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp_dp;

        ++cache_len;
    }

    return cache[needed_len];
}
