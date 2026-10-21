/*
 * A260603  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000101 00010001 or 00010101.
 * https://oeis.org/A260603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +4*a(n-3) +3*a(n-4) +2*a(n-6) +2*a(n-7) -4*a(n-8)
 *            + 3*a(n-9) +4*a(n-10) -a(n-11) +a(n-12) +a(n-13) initial values
 *            (n = 1 … 14): 58, 118, 250, 532, 1136, 2480, 5418, 11776, 25694,
 *            56138, 122376, 266782, 582226, 1270070
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 52, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t cap   = 0;
    /* initial values for n = 1 … 14 */
    static const seq_t init[14] = {
        58ULL, 118ULL, 250ULL, 532ULL, 1136ULL, 2480ULL, 5418ULL, 11776ULL,
        25694ULL, 56138ULL, 122376ULL, 266782ULL, 582226ULL, 1270070ULL
    };

    if (n == 0)
        return 0;                 /* undefined for n < 1, return 0 */
    /* grow memo array to hold values up to n */
    if (memo == NULL) {
        /* start with capacity at least 15 to hold up to n = 14 */
        cap = (size_t) ( (n < 15) ? 15 : n + 1 );
        memo = (seq_t *) malloc(cap * sizeof(seq_t));
        if (memo == NULL)
            return 0;             /* memory allocation failed */
        /* initialise known first 14 terms */
        for (size_t i = 1; i <= 14 && i < cap; ++i)
            memo[i] = init[i-1];
        for (size_t i = 15; i < cap; ++i)
            memo[i] = 0;
    } else if (n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *) realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;             /* memory allocation failed */
        memo = tmp;
        /* initialise new slots to 0 (except for first 14 values) */
        for (size_t i = cap; i < new_cap; ++i)
            memo[i] = 0;
        cap = new_cap;
    }

    /* if n <= 14, the value is already set */
    if (n <= 14)
        return memo[n];

    /* compute missing terms up to n */
    for (idx_t i = 15; i <= n; ++i) {
        seq_t res = memo[i-1];
        res += 4 * memo[i-3];
        res += 3 * memo[i-4];
        res += 2 * memo[i-6];
        res += 2 * memo[i-7];
        res -= 4 * memo[i-8];
        res += 3 * memo[i-9];
        res += 4 * memo[i-10];
        res -= memo[i-11];
        res += memo[i-12];
        res += memo[i-13];
        memo[i] = res;
    }
    return memo[n];
}
