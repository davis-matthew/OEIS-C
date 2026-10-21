/*
 * A258685  Number of (n+3) X (1+3) 0..1 arrays with no 4 X 4 subblock
 *          diagonal sum less than the antidiagonal sum.
 * https://oeis.org/A258685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+3)×4 binary arrays with no 4×4 subblock
 *            whose diagonal sum is less than its anti‑diagonal sum (OEIS
 *            A258685)
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 19, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Transition table: from a 12‑bit state (last 3 rows) to a new state when
   a new row r (4 bits) is appended.
   next_state[s][r] = new state, or -1 if the 4×4 block violates the rule. */
static int16_t next_state[4096][16];
static bool trans_init = false;

/* Build the transition table once */
static void init_transition(void)
{
    for (int s = 0; s < 4096; ++s)
    {
        int a = (s >> 8) & 0xF;   /* first (oldest) row */
        int b = (s >> 4) & 0xF;   /* second row    */
        int c =  s        & 0xF;   /* third row     */
        for (int r = 0; r < 16; ++r)
        {
            /* Diagonal sums for the 4×4 block formed by rows a,b,c,r */
            int sum_main = ((a >> 0) & 1) + ((b >> 1) & 1)
                         + ((c >> 2) & 1) + ((r >> 3) & 1);
            int sum_anti = ((a >> 3) & 1) + ((b >> 2) & 1)
                         + ((c >> 1) & 1) + ((r >> 0) & 1);
            if (sum_main >= sum_anti)
            {
                int ns = ((s << 4) & 0xFFF) | r;   /* shift left by 4 bits and add r */
                next_state[s][r] = (int16_t)ns;
            }
            else
                next_state[s][r] = -1;
        }
    }
}

/* DP buffers */
static seq_t dp_cur[4096];
static seq_t dp_next[4096];

/* Memoisation array for sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache array */
static idx_t cached_up_to = 0; /* highest n already computed */

/* Main routine to compute a(n) */
seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n < 1 is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    if (!trans_init)
    {
        init_transition();
        trans_init = true;
    }

    /* Ensure cache can hold index n */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return if already computed */
    if (n <= cached_up_to)
        return cache[n];

    /* Initialise DP buffers once (after first call) */
    if (cached_up_to == 0)
    {
        seq_t one = (seq_t)1;
        for (int i = 0; i < 4096; ++i)
            dp_cur[i] = one;      /* all 3‑row states occur exactly once */
    }

    /* Compute terms from cached_up_to+1 up to n */
    for (idx_t k = cached_up_to + 1; k <= n; ++k)
    {
        /* Clear next buffer */
        for (int i = 0; i < 4096; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition */
        for (int s = 0; s < 4096; ++s)
        {
            seq_t cnt = dp_cur[s];
            if (cnt == (seq_t)0)
                continue;
            for (int r = 0; r < 16; ++r)
            {
                int ns = next_state[s][r];
                if (ns >= 0)
                    dp_next[ns] = dp_next[ns] + cnt;
            }
        }

        /* Sum all states to obtain a(k) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 4096; ++i)
            sum = sum + dp_next[i];
        cache[k] = sum;

        /* Prepare for next iteration */
        for (int i = 0; i < 4096; ++i)
            dp_cur[i] = dp_next[i];
    }

    cached_up_to = n;
    return cache[n];
}
