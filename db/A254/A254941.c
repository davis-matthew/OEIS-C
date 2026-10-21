/*
 * A254941  Number of length n 1..(2+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the a(n) term of OEIS A254941.
 *
 *  a(n) is the number of length‑n arrays with entries in {1,2,3} such that
 *  every leading partial sum is divisible by at least one of 2, 3, 5, 7,
 *  or 11.  The algorithm is dynamic‑programming over residues modulo
 *  LCM(2,3,5,7,11)=2310.  It keeps the results for all n ≤ current
 *  length cached so that calls in any order are O(1) after the first
 *  computation.
 *
 *  The implementation satisfies the required constraints for both C
 *  and C++ builds, with seq_t being either an unsigned integer type or
 *  an arbitrary‑precision class.
 */

#include <stdlib.h>
#include <stdbool.h>

/* LCM of the divisors 2,3,5,7,11 */
#define MOD 2310

/* table of residues that satisfy the divisibility condition */
static bool good[MOD];

/* DP arrays: number of sequences of the current length that end
   with a given residue modulo MOD.  The arrays are of fixed size
   and are static so that their elements are default‑constructed
   in both C and C++. */
static seq_t dp_arr[MOD];
static seq_t dp_next_arr[MOD];
static seq_t *dp      = dp_arr;
static seq_t *dp_next = dp_next_arr;

/* Cache for a(1)..a(max_n) */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;
static idx_t  current_len = 0;

/* Initialise the static data once */
static void
init_once(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Pre‑compute which residues satisfy the condition */
    for (idx_t i = 0; i < MOD; ++i) {
        good[i] = (i % 2 == 0) || (i % 3 == 0) ||
                  (i % 5 == 0) || (i % 7 == 0) ||
                  (i % 11 == 0);
    }

    /* Zero initial DP arrays */
    for (idx_t i = 0; i < MOD; ++i) {
        dp[i]      = 0;
        dp_next[i] = 0;
    }
    /* Empty sequence has sum 0 */
    dp[0] = 1;
    current_len = 0;
}

/* Ensure we have storage for a[0..n] (a[0] is unused) */
static void
ensure_cache(idx_t n)
{
    if (!a_cache || a_cap <= n) {
        size_t new_cap = (size_t)n + 1;  /* indices 0..n */
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!a_cache) exit(1);
        a_cap = new_cap;
    }
}

/* Compute a(k) for all k ≤ n, filling the cache if necessary */
static void
compute_up_to(idx_t n)
{
    init_once();
    if (n <= current_len) return;

    for (idx_t len = current_len + 1; len <= n; ++len) {
        /* clear dp_next array */
        for (idx_t r = 0; r < MOD; ++r) dp_next[r] = 0;

        /* transition from sequences of length len‑1 to len */
        for (idx_t r_prev = 0; r_prev < MOD; ++r_prev) {
            if (dp[r_prev] == 0) continue;
            for (int v = 1; v <= 3; ++v) {
                idx_t r_new = r_prev + (idx_t)v;
                if (r_new >= MOD) r_new -= MOD;
                if (!good[r_new]) continue;
                dp_next[r_new] += dp[r_prev];
            }
        }

        /* sum over all residues for a(len) */
        seq_t sum = 0;
        for (idx_t r = 0; r < MOD; ++r) sum += dp_next[r];

        ensure_cache(len);
        a_cache[len] = sum;

        /* swap dp and dp_next for the next iteration */
        seq_t *tmp = dp;
        dp        = dp_next;
        dp_next   = tmp;
    }
    current_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – safe behaviour */
    compute_up_to(n);
    return a_cache[n];
}
