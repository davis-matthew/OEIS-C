/*
 * A383208  Expansion of e.g.f. (exp(f(x)) - 1)^3 / 6, where f(x) = (exp(2*x)
 *          - 1)/2.
 * https://oeis.org/A383208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=3..n} 2^(n-k) * Stirling2(n,k) * Stirling2(k,3).
 *            Computation uses recurrence for Stirling numbers of the second
 *            kind and the closed form for Stirling2(k,3) = (3^k - 3*2^k +
 *            3)/6
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are provided by the build environment. */

static seq_t *S = NULL;          /* current Stirling row S(k) for k=0..n */
static seq_t *pow2 = NULL;       /* 2^i */
static seq_t *pow3 = NULL;       /* 3^i */
static seq_t *carr = NULL;       /* Stirling2(k,3) for k>=3 */
static seq_t *a_arr = NULL;      /* cache of a(n) values */

static idx_t allocated = 0;      /* size of allocated arrays (>= requested+1) */
static idx_t computed = 0;       /* highest n for which a(n) is known */

/* Ensure that the four arrays have room for indices up to n inclusive.  */
static void ensure_capacity(idx_t n)
{
    if (allocated > n)
        return;

    idx_t new_cap = (n > allocated * 2 ? n + 1 : allocated * 2 + 1);
    if (S == NULL) {
        S = (seq_t*)malloc(new_cap * sizeof(seq_t));
        pow2 = (seq_t*)malloc(new_cap * sizeof(seq_t));
        pow3 = (seq_t*)malloc(new_cap * sizeof(seq_t));
        carr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        a_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        S = (seq_t*)realloc(S, new_cap * sizeof(seq_t));
        pow2 = (seq_t*)realloc(pow2, new_cap * sizeof(seq_t));
        pow3 = (seq_t*)realloc(pow3, new_cap * sizeof(seq_t));
        carr = (seq_t*)realloc(carr, new_cap * sizeof(seq_t));
        a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
    }

    /* Initialize the newly allocated slots to zero.  */
    for (idx_t i = allocated; i < new_cap; ++i) {
        S[i] = 0;
        pow2[i] = 0;
        pow3[i] = 0;
        carr[i] = 0;
        a_arr[i] = 0;
    }
    allocated = new_cap;
}

/* Initialize the data for n=0.  */
static void init_once(void)
{
    if (computed == 0) {
        /* Base row: S(0,0)=1.  pow2[0]=pow3[0]=1; carr[0] unused.  */
        S[0] = 1;
        pow2[0] = 1;
        pow3[0] = 1;
        carr[0] = 0;
        a_arr[0] = 0;
        pow2[1] = 0;  /* placeholders; will be set when needed */
        pow3[1] = 0;
        carr[1] = 0;
        a_arr[1] = 0;
        a_arr[2] = 0;
        computed = 0;  /* no values computed yet beyond base */
    }
}

/* Main sequence generator.  */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;  /* a(0), a(1), a(2) are 0 */

    /* Allocate space if necessary.  */
    ensure_capacity(n);
    init_once();

    /* Compute new rows up to the requested n.  */
    for (idx_t cur = computed + 1; cur <= n; ++cur) {
        /* Pre‑compute powers of 2 and 3 for this index.  */
        pow2[cur] = pow2[cur - 1] * (seq_t)2;
        pow3[cur] = pow3[cur - 1] * (seq_t)3;

        /* Stirling2(cur,3) = (3^cur - 3*2^cur + 3)/6  */
        carr[cur] = (pow3[cur] - (seq_t)3 * pow2[cur] + (seq_t)3) / (seq_t)6;

        /* Update Stirling numbers S(row=cur, k) using recurrence.  */
        /* We go backwards to keep the old row values untouched.  */
        for (idx_t k = cur; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        S[0] = 0;  /* S(cur,0) = 0 for cur>0 */

        /* Compute a(cur).  */
        seq_t sum = 0;
        for (idx_t k = 3; k <= cur; ++k) {
            seq_t term = pow2[cur - k] * S[k];
            term = term * carr[k];
            sum += term;
        }
        a_arr[cur] = sum;
    }

    computed = n;
    return a_arr[n];
}
