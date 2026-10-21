/*
 * A258904  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          median plus antidiagonal median nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A258904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 binary matrices for which, for every
 *            3×3 subblock (starting at rows i,i+1,i+2 and columns 0 or 1) the
 *            sum of the median of the main diagonal and the median of the
 *            anti‑diagonal is nondecreasing horizontally (left ≤ right) and
 *            vertically (below ≥ above). Computed by dynami...
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 23, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* DP arrays: dp_cur[a][b][s0][s1] counts arrays whose last
   two rows are a,b (0..15) and whose last 3×3 subblock
   (starting two rows above) has left sum s0 and right sum s1. */
static seq_t dp_cur[16][16][3][3];
static seq_t dp_next[16][16][3][3];

/* Static memoisation for n ≤ 10000. */
#define MAX_STATIC 10000
static seq_t results_static[MAX_STATIC + 1] = {0};

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> results_vec;
#else
static seq_t *results_dyn = NULL;
static idx_t results_dyn_size = 0;
#endif

static idx_t max_computed = 0;                /* largest n that has been computed */

/* Return the majority of three bits. */
static inline int majority3(int x, int y, int z)
{
    return ((x + y + z) >= 2);
}

/* Initialise dp_cur for n = 1 (first 3 rows).  We store the state
   with the last two rows (b,c). */
static void init_dp_for_n1(void)
{
    for (int a = 0; a < 16; ++a)
    {
        int a0 = (a >> 0) & 1, a1 = (a >> 1) & 1,
            a2 = (a >> 2) & 1, a3 = (a >> 3) & 1;
        for (int b = 0; b < 16; ++b)
        {
            int b0 = (b >> 0) & 1, b1 = (b >> 1) & 1,
                b2 = (b >> 2) & 1, b3 = (b >> 3) & 1;
            for (int c = 0; c < 16; ++c)
            {
                int c0 = (c >> 0) & 1, c1 = (c >> 1) & 1,
                    c2 = (c >> 2) & 1, c3 = (c >> 3) & 1;
                int s0 = majority3(a0, b1, c2) + majority3(a2, b1, c0);
                int s1 = majority3(a1, b2, c3) + majority3(a3, b2, c1);
                if (s0 <= s1) /* horizontal nondecreasing */
                    dp_cur[b][c][s0][s1] += (seq_t)1;
            }
        }
    }
}

/* Memoisation helpers. */
static inline void store_result(idx_t n, seq_t val)
{
    if (n <= MAX_STATIC)
    {
        results_static[n] = val;
    }
    else
    {
#ifdef __cplusplus
        if (results_vec.size() <= n) results_vec.resize(n + 1);
        results_vec[n] = val;
#else
        if (n >= results_dyn_size)
        {
            idx_t new_size = n + 1;
            seq_t *tmp = (seq_t*)realloc(results_dyn, new_size * sizeof(seq_t));
            if (!tmp) abort();
            results_dyn = tmp;
            results_dyn_size = new_size;
        }
        results_dyn[n] = val;
#endif
    }
}

static inline seq_t get_result(idx_t n)
{
    if (n <= MAX_STATIC)
        return results_static[n];
#ifdef __cplusplus
    return results_vec[n];
#else
    return results_dyn[n];
#endif
}

/* Extend the DP to compute a(n) up to target_n (target_n > max_computed). */
static void extend_to(idx_t target_n)
{
    for (idx_t n = max_computed + 1; n <= target_n; ++n)
    {
        /* clear dp_next */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int s0 = 0; s0 < 3; ++s0)
                    for (int s1 = 0; s1 < 3; ++s1)
                        dp_next[a][b][s0][s1] = 0;

        for (int a = 0; a < 16; ++a)
        {
            int a0 = (a >> 0) & 1, a1 = (a >> 1) & 1,
                a2 = (a >> 2) & 1, a3 = (a >> 3) & 1;
            for (int b = 0; b < 16; ++b)
            {
                int b0 = (b >> 0) & 1, b1 = (b >> 1) & 1,
                    b2 = (b >> 2) & 1, b3 = (b >> 3) & 1;
                for (int s0_prev = 0; s0_prev < 3; ++s0_prev)
                {
                    for (int s1_prev = 0; s1_prev < 3; ++s1_prev)
                    {
                        seq_t val = dp_cur[a][b][s0_prev][s1_prev];
                        if (!val) continue;
                        for (int c = 0; c < 16; ++c)
                        {
                            int c0 = (c >> 0) & 1, c1 = (c >> 1) & 1,
                                c2 = (c >> 2) & 1, c3 = (c >> 3) & 1;
                            int s0_cur = majority3(a0, b1, c2) + majority3(a2, b1, c0);
                            int s1_cur = majority3(a1, b2, c3) + majority3(a3, b2, c1);
                            if (s0_cur >= s0_prev && s1_cur >= s1_prev && s0_cur <= s1_cur)
                                dp_next[b][c][s0_cur][s1_cur] += val;
                        }
                    }
                }
            }
        }

        /* move dp_next into dp_cur */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int s0 = 0; s0 < 3; ++s0)
                    for (int s1 = 0; s1 < 3; ++s1)
                        dp_cur[a][b][s0][s1] = dp_next[a][b][s0][s1];

        /* total number for current n */
        seq_t total = 0;
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int s0 = 0; s0 < 3; ++s0)
                    for (int s1 = 0; s1 < 3; ++s1)
                        total += dp_cur[a][b][s0][s1];

        store_result(n, total);
        max_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (max_computed == 0)   /* first call: initialise the base case */
    {
        init_dp_for_n1();

        /* total for n = 1 */
        seq_t total = 0;
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int s0 = 0; s0 < 3; ++s0)
                    for (int s1 = 0; s1 < 3; ++s1)
                        total += dp_cur[a][b][s0][s1];
        store_result(1, total);
        max_computed = 1;
    }

    if (n > max_computed)
        extend_to(n);

    return get_result(n);
}
