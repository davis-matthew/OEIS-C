/*
 * A378005  Number of permutations (p(1),p(2),...,p(n)) of (1,2,...,n) such
 *          that p(i)-i is in {-2,4,5} for all i=1,...,n.
 * https://oeis.org/A378005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-3) + 2*a(n-6) + 3*a( n-7 )
 *            - 2*a(n-9) - 2*a(n-10) - a(n-12)
 *            - 2*a(n-13) - 3*a(n-14) + a(n-15) + a(n-16)
 *            + a(n-17) + a(n-21),    a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=0, a(5)=0, a(6)=1, a(7)=1, a(8)=0, a(9)=0, a(10)=0,
 *            a(11)=0, a(12)=1, a(13)=3, a(14)=1, a(15)=0, a(16)=0,
 *            a(17)=0, a(18)=1, a(19)=6, a(20)=6
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t base_vals[21] = {
    1ULL, /* 0 */
    0ULL, /* 1 */
    0ULL, /* 2 */
    0ULL, /* 3 */
    0ULL, /* 4 */
    0ULL, /* 5 */
    1ULL, /* 6 */
    1ULL, /* 7 */
    0ULL, /* 8 */
    0ULL, /* 9 */
    0ULL, /*10 */
    0ULL, /*11 */
    1ULL, /*12 */
    3ULL, /*13 */
    1ULL, /*14 */
    0ULL, /*15 */
    0ULL, /*16 */
    0ULL, /*17 */
    1ULL, /*18 */
    6ULL, /*19 */
    6ULL  /*20 */
};

seq_t a(idx_t n)
{
    /* Handle small index directly */
    if (n < 21)
        return base_vals[n];

    /* Cache of computed values: memo[0..current_n] */
    static seq_t *memo = NULL;
    static idx_t memo_n = 0;       /* number of slots allocated (= memo_n) */
    idx_t i;

    /* Allocate or grow the cache if needed */
    if (n >= memo_n) {
        idx_t new_n = n + 1;
        seq_t *tmp = NULL;
        if (memo == NULL) {
            tmp = (seq_t *)malloc(new_n * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(memo, new_n * sizeof(seq_t));
        }
        memo = tmp;

        /* Fill newly allocated slots */
        for (i = memo_n; i <= n; ++i) {
            if (i < 21) {
                memo[i] = base_vals[i];
            } else {
                /* Compute using the provided recurrence */
                seq_t term =
                    memo[i -  3];                 /* a(i-3) */
                term += 2ULL * memo[i -  6];     /* 2*a(i-6) */
                term += 3ULL * memo[i -  7];     /* 3*a(i-7) */

                /* Add later-positive terms first */
                term += memo[i - 15];
                term += memo[i - 16];
                term += memo[i - 17];
                term += memo[i - 21];

                /* Subtract the negative terms */
                seq_t neg = 2ULL * memo[i -  9];
                neg += 2ULL * memo[i - 10];
                neg += memo[i - 12];
                neg += 2ULL * memo[i - 13];
                neg += 3ULL * memo[i - 14];
                term -= neg;                      /* safe: term >= neg */

                memo[i] = term;
            }
        }
        memo_n = new_n;
    }

    return memo[n];
}
