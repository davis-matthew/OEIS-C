/*
 * A317779  Number of equivalence classes of binary words of length n for the
 *          set of subwords {010, 101, 10110}.
 * https://oeis.org/A317779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 71, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A317779: Number of equivalence classes of binary words of length n
   for the subword set {010, 101, 10110}.
   Recurrence:
     a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=7, a(5)=14, a(6)=26, a(7)=47,
     a(8)=86, a(9)=160,
     a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-6)+a(n-7)-a(n-8)-a(n-9)-a(n-10)
   for all n ≥ 10.  */

/* ---- Standard headers for memory management ---------------------- */
#include <stdlib.h>

/* ---- API function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initial values for n = 0 … 9.  They are reused for all calls. */
    static const seq_t init_vals[10] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)7,
        (seq_t)14, (seq_t)26, (seq_t)47, (seq_t)86, (seq_t)160
    };

    /* Static memoisation storage.  dp[j] holds a(j) for all j already
       computed.  The array grows automatically to accommodate requested
       indices. */
    static seq_t *dp       = NULL;    /* dynamic array of seq_t elements */
    static idx_t  capacity = 0;      /* actual allocated size (number of elems) */
    static idx_t  max_idx  = 0;      /* highest index for which dp[max_idx] is valid */

    /* ---- First call: initialise storage with the 10 known terms. ------- */
    if (dp == NULL) {
        /* Allocate space for at least the first 10 terms. */
        capacity = 10;
        dp = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;  /* out of memory, return 0 */
        for (idx_t i = 0; i < 10; ++i)
            dp[i] = init_vals[i];
        max_idx = 9;   /* last computed index */
    }

    /* ---- Handle requests for indices below the pre‑initialized range -------- */
    if (n <= 9) return dp[n];

    /* ---- Ensure the array is large enough for requested n ---------- */
    if ((idx_t)(n + 1) > capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap < (idx_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* out of memory */
        dp = tmp;
        capacity = new_cap;
    }

    /* ---- Compute missing terms up to n using the recurrence --------- */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        /* All indices i-1 … i-10 are guaranteed to be valid because i ≥ 10. */
        seq_t val = dp[i - 1] + dp[i - 2] + dp[i - 3];
        val += dp[i - 6] + dp[i - 7];
        val -= dp[i - 8] + dp[i - 9] + dp[i - 10];
        dp[i] = val;
    }
    max_idx = n;  /* update the furthest computed index */

    return dp[n];
}
