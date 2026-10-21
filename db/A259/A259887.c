/*
 * A259887  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00000011.
 * https://oeis.org/A259887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A259887: Number of (n+2)×(1+2) 0..1 arrays with each 3×3 subblock
 * having clockwise perimeter pattern 00000000, 00000001, or 00000011.
 *
 * The sequence satisfies the recurrence
 *   a(1)=34,  a(2)=67,  a(3)=215,  a(4)=499
 *   a(n) = a(n-1) + 5*a(n-3) + 3*a(n-4)   for n ≥ 5
 *
 * This implementation memoises the values so that repeated calls
 * (in any order) are efficient. The memoised data is kept in a static
 * array that expands as needed. The recurrence is computed using
 * standard arithmetic on the seq_t type; if seq_t is an arbitrary
 * precision class (e.g. boost::multiprecision::cpp_int) the overloaded
 * operators will be used automatically.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle out‑of‑range requests silently. */
    if (n < 1) return 0;

    /* Static memoisation table. */
    static seq_t *memo = NULL;      /* ptr to array (index 0 unused) */
    static size_t  cap  = 0;        /* current capacity (in terms) */
    static size_t  last = 0;        /* highest index computed so far */

    /* Initialise the table on the first call. */
    if (memo == NULL) {
        cap = 8;
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!memo) return 0;       /* out of memory – not expected in tests */
        memo[0] = 0;               /* dummy value for index 0 */

        /* Base cases (as given). */
        memo[1] = (seq_t)34;
        memo[2] = (seq_t)67;
        memo[3] = (seq_t)215;
        memo[4] = (seq_t)499;

        last = 4;
    }

    /* Grow the memo table if necessary. */
    while (last < (size_t)n) {
        if (last + 1 >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            memo = tmp;
            cap = new_cap;
        }

        size_t idx = last + 1;  /* compute next index */
        seq_t val;

        if (idx <= 4) {
            /* Already initialised; nothing to do. */
            val = memo[idx];
        } else {
            /* Apply the recurrence. */
            val = memo[idx - 1];
            val += memo[idx - 3] * (seq_t)5;
            val += memo[idx - 4] * (seq_t)3;
        }

        memo[idx] = val;
        last = idx;
    }

    return memo[n];
}
