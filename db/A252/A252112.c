/*
 * A252112  Number of (n+2) X (6+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum equal to 0 2 3 6 or 7 and every 3 X 3 diagonal and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-3) - 32*a(n-6) for n > 11; Initial values (n = 1
 *            .. 11): a(1)=2058, a(2)=788, a(3)=1634, a(4)=2658, a(5)=6070,
 *            a(6)=9834, a(7)=17772, a(8)=38610, a(9)=70968, a(10)=132528,
 *            a(11)=284488
 * Range:     u8 none, u16 n <= 8, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *memo = NULL;          /* memoized terms, 1‑based index */
static size_t memo_cap = 0;         /* allocated array size (>=1‑based + 1) */
static idx_t  computed_max = 0;     /* highest index that has been computed */

/* Allocate (or expand) the memo array and initialise the first 11 terms. */
static void ensure_memo(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* array size needed (index 0 unused) */
    if (!memo)                              /* first time: allocate and init */
    {
        size_t cap = (needed > 12 ? needed : 12);
        memo     = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        memo_cap = cap + 1;

        /* initialise 1 … 11 (index 0 remains 0) */
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)2058ULL;
        memo[2] = (seq_t)788ULL;
        memo[3] = (seq_t)1634ULL;
        memo[4] = (seq_t)2658ULL;
        memo[5] = (seq_t)6070ULL;
        memo[6] = (seq_t)9834ULL;
        memo[7] = (seq_t)17772ULL;
        memo[8] = (seq_t)38610ULL;
        memo[9] = (seq_t)70968ULL;
        memo[10] = (seq_t)132528ULL;
        memo[11] = (seq_t)284488ULL;
        computed_max = 11;
    }
    else if (needed > memo_cap)            /* expand array if necessary */
    {
        size_t new_cap = needed;
        if (new_cap < memo_cap * 2) new_cap = memo_cap * 2;
        seq_t *tmp = (seq_t *)realloc(memo, (new_cap + 1) * sizeof(seq_t));
        memo = tmp;
        memo_cap = new_cap + 1;
    }
}

/* Compute a(n) with memoisation.  The function works for any call order
   and for any n ≥ 1.  Results are exact whenever they fit into seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* Undefined for n=0 – return 0 gracefully */
    /* Ensure we have room and the initial 11 terms. */
    ensure_memo(n);

    /* If the term is already known, return it. */
    if (n <= computed_max) return memo[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        memo[i] = ((seq_t)12 * memo[i - 3]) - ((seq_t)32 * memo[i - 6]);
    }
    computed_max = n;
    return memo[n];
}
