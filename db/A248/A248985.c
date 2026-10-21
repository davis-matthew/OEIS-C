/*
 * A248985  Number of length n+4 0..6 arrays with every five consecutive terms
 *          having two times the sum of some three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A248985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays over {0..6} such that for
 *            every consecutive 5-tuple the relation 2·sum(3) = 3·sum(2)
 *            holds. Computed by dynamic programming over the 4‑tuples that
 *            form the overlap of consecutive windows
 * Range:     u8 none, u16 n <= 8, u32 n <= 29, u64 n <= 69, u128 n <= 149,
 *            big any n
 * Helpers:   idx4, idx5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/idx4.h"
#include "../utils/idx5.h"

/* Size of state space: 7^4 = 2401 4‑tuples */
#define N4 2401
/* Size of 5‑tuple space: 7^5 = 16807 */
#define N5 16807

/* Precomputed tables */
static bool valid5[N5];
static int  next_state[N4][7];
static unsigned short nnext[N4];

/* DP containers */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Memoised sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;      /* number of allocated cache entries */
static idx_t  cached_n  = 0;      /* maximum n already computed */

/* One‑time initialisation flag */
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Initialise lookup tables and DP arrays.  Must be called once.     */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* --- 1. Acceptable 5‑tuples ----------------------------------- */
    for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 7; ++b)
            for (int c = 0; c < 7; ++c)
                for (int d = 0; d < 7; ++d)
                    for (int e = 0; e < 7; ++e)
    {
        int i5 = idx5(a,b,c,d,e);
        int S = a+b+c+d+e;
        if (S % 5 != 0) { valid5[i5] = false; continue; }
        int target = (3 * S) / 5;           /* required sum of 3 elements */
        int v[5] = {a,b,c,d,e};
        bool ok = false;
        for (int i = 0; i < 5 && !ok; ++i)
            for (int j = i+1; j < 5 && !ok; ++j)
                for (int k = j+1; k < 5; ++k)
                {
                    if (v[i]+v[j]+v[k] == target) { ok = true; break; }
                }
        valid5[i5] = ok;
    }

    /* --- 2. Successor lists for each 4‑tuple ------------------------ */
    memset(nnext, 0, sizeof(nnext));
    for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 7; ++b)
            for (int c = 0; c < 7; ++c)
                for (int d = 0; d < 7; ++d)
    {
        int state = idx4(a,b,c,d);
        for (int e = 0; e < 7; ++e)
        {
            if (!valid5[idx5(a,b,c,d,e)]) continue;
            int ns = idx4(b,c,d,e);      /* new last 4‑tuple */
            int k = nnext[state]++;
            next_state[state][k] = ns;
        }
    }

    /* --- 3. Allocate DP vectors ------------------------------------ */
    dp_cur  = (seq_t*)calloc(N4, sizeof(seq_t));
    dp_next = (seq_t*)malloc(N4 * sizeof(seq_t));
    for (int i = 0; i < N4; ++i) dp_cur[i] = 1;   /* length 4 sequences */
}

/* ------------------------------------------------------------------ */
/* Return a(n) for n >= 1.  Calls init  automatically.              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* value not defined for n < 1 */

    init();   /* ensure tables are ready */

    /* Allocate cache if necessary */
    if ((size_t)n > cache_len)
    {
        cache_len = (size_t)n;
        seq_cache = (seq_t*)realloc(seq_cache, cache_len * sizeof(seq_t));
    }

    /* If requested term is already available, return it */
    if (cached_n >= n)
        return seq_cache[n-1];

    /* Compute terms from (cached_n + 1) up to n */
    for (idx_t step = cached_n + 1; step <= n; ++step)
    {
        /* Transition */
        memset(dp_next, 0, N4 * sizeof(seq_t));
        for (int st = 0; st < N4; ++st)
        {
            seq_t val = dp_cur[st];
            if (val == 0) continue;
            unsigned short k = nnext[st];
            for (unsigned short i = 0; i < k; ++i)
            {
                int ns = next_state[st][i];
                dp_next[ns] += val;
            }
        }

        /* Sum over all final states to obtain a(step) */
        seq_t total = 0;
        for (int st = 0; st < N4; ++st) total += dp_next[st];
        seq_cache[step-1] = total;

        /* Prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    cached_n = n;
    return seq_cache[n-1];
}
