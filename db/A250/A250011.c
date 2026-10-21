/*
 * A250011  Number of length n+5 0..5 arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 arrays over alphabet {0,1,2,3,4,5}
 *            in which no six consecutive terms contain a partition into two
 *            groups of three terms such that the maximum of one group equals
 *            the minimum of the other. The sequence is computed by a
 *            transfer‑matrix DP on 5‑digit suffixes, memoised pe...
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 23, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Build‑time helper */
#if defined(__cplusplus)
#  define ALLOC_SEQ_T(n) new seq_t[(n)]
#  define FREE_SEQ_T(p) delete[] (p)
#else
#  define ALLOC_SEQ_T(n) ((seq_t*)calloc((n), sizeof(seq_t)))
#  define FREE_SEQ_T(p) free(p)
#endif

/* Transition parameters */
#define STATES 7776   /* 6^5 */
#define ALPHA  6
#define TOP4 1296     /* 6^4 */

/* Transition tables – built once */
static bool     allowed[STATES][ALPHA];
static uint16_t next_state[STATES][ALPHA];
static bool     tables_built = false;

/* DP working vectors – allocated once */
static seq_t *dp      = NULL;
static seq_t *dp_next = NULL;
static bool   dp_allocated = false;

/* Cache of results (1‑based indexing) */
static seq_t *a_cache   = NULL;
static size_t cache_cap = 0;   /* number of entries allocated (>= maxima) */

/* Maximum n that has already been computed */
static idx_t max_comp = 0;

/* ------------------------------------------------------------ */
/* Helper: ensure cache can accommodate index n (1‑based). */
static void grow_cache(idx_t needed)
{
    if (needed < cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_cache = ALLOC_SEQ_T(new_cap + 1);  /* +1 for 1‑based index */
    if (a_cache) {
        /* copy existing entries */
        for (size_t i = 1; i <= cache_cap; ++i) new_cache[i] = a_cache[i];
        FREE_SEQ_T(a_cache);
    }
    a_cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Build transition tables once. */
static void build_transition(void)
{
    for (size_t s = 0; s < STATES; ++s) {
        uint8_t dig[5];
        size_t t = s;
        for (int k = 4; k >= 0; --k) {
            dig[k] = t % 6;
            t /= 6;
        }

        for (int d = 0; d < ALPHA; ++d) {
            int w[6];
            for (int i = 0; i < 5; ++i) w[i] = dig[i];
            w[5] = d;

            /* sort the 6‑tuple (insertion sort is fine here) */
            for (int i = 1; i < 6; ++i) {
                int key = w[i];
                int j = i - 1;
                while (j >= 0 && w[j] > key) {
                    w[j + 1] = w[j];
                    --j;
                }
                w[j + 1] = key;
            }
            /* window allowed iff third and fourth smallest differ */
            allowed[s][d] = (w[2] != w[3]);

            /* next state after dropping oldest digit */
            next_state[s][d] = (uint16_t)((s % TOP4) * ALPHA + (uint16_t)d);
        }
    }
}

/* ------------------------------------------------------------ */
/* Initialise DP vectors once. */
static void init_dp(void)
{
    if (!dp_allocated) {
        dp = ALLOC_SEQ_T(STATES);
        dp_next = ALLOC_SEQ_T(STATES);
        for (size_t i = 0; i < STATES; ++i) dp[i] = 1U;
        for (size_t i = 0; i < STATES; ++i) dp_next[i] = 0U;
        dp_allocated = true;
    }
}

/* ------------------------------------------------------------ */
/* Compute all terms up to target n (inclusive). */
static void compute_up_to(idx_t target)
{
    if (target <= max_comp) return;

    if (!dp_allocated) init_dp();
    if (!tables_built) { build_transition(); tables_built = true; }

    for (idx_t step = max_comp + 1; step <= target; ++step) {
        /* zero dp_next */
        for (size_t i = 0; i < STATES; ++i) dp_next[i] = 0U;

        /* transition */
        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = dp[s];
            if (!val) continue;  /* skip zero states */
            for (int d = 0; d < ALPHA; ++d) {
                if (!allowed[s][d]) continue;
                size_t ns = next_state[s][d];
                dp_next[ns] += val;
            }
        }

        /* swap dp and dp_next */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;

        /* sum to obtain a(step) */
        seq_t total = 0U;
        for (size_t i = 0; i < STATES; ++i) total += dp[i];

        grow_cache(step);
        a_cache[step] = total;
    }
    max_comp = target;
}

/* ------------------------------------------------------------ */
/* Public API required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0U;  /* undefined for n < 1 */

    compute_up_to(n);
    return a_cache[n];
}
