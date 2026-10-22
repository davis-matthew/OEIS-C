/*
 * A320397  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A320397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 11*a(n-2) - a(n-3) - 8*a(n-4) - a(n-5) -
 *            26*a(n-6) - 5*a(n-7) + 18*a(n-8) + 5*a(n-9) - 5*a(n-10) -
 *            4*a(n-11) - a(n-12) Initial values: a(1)=4, a(2)=28, a(3)=142,
 *            a(4)=732, a(5)=3694, a(6)=18754, a(7)=94842, a(8)=480321,
 *            a(9)=2431390, a(10)=12309913, a(11)=62320210, a(12)=3155085...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 14 initial terms (1‑indexed) */
    static const seq_t init[15] = {
        0ULL,          /* dummy 0 index */
        4ULL,
        28ULL,
        142ULL,
        732ULL,
        3694ULL,
        18754ULL,
        94842ULL,
        480321ULL,
        2431390ULL,
        12309913ULL,
        62320210ULL,
        315508545ULL,
        1597316261ULL,
        8086701105ULL
    };

    /* for n <= 14 we can return the pre‑computed value */
    if (n <= 14) {
        return init[(size_t)n];
    }

    /* allocate an array of size n+1; construct objects for C++ */
    seq_t *a_seq;
#ifdef __cplusplus
    a_seq = new seq_t[(size_t)(n + 1)];
#else
    a_seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (a_seq == NULL) { /* allocation failed */
        return 0;
    }
#endif

    /* initialise known values (1 to 14) */
    for (idx_t i = 1; i <= 14; ++i) {
        a_seq[(size_t)i] = init[(size_t)i];
    }

    /* compute terms from 15 up to n using the recurrence */
    for (idx_t k = 15; k <= n; ++k) {
        seq_t pos = 3 * a_seq[(size_t)(k - 1)] +
                    11 * a_seq[(size_t)(k - 2)] +
                    18 * a_seq[(size_t)(k - 8)] +
                    5 * a_seq[(size_t)(k - 9)];
        seq_t neg = a_seq[(size_t)(k - 3)] +
                    8 * a_seq[(size_t)(k - 4)] +
                    a_seq[(size_t)(k - 5)] +
                    26 * a_seq[(size_t)(k - 6)] +
                    5 * a_seq[(size_t)(k - 7)] +
                    5 * a_seq[(size_t)(k - 10)] +
                    4 * a_seq[(size_t)(k - 11)] +
                    a_seq[(size_t)(k - 12)];
        a_seq[(size_t)k] = pos - neg;
    }

    seq_t result = a_seq[(size_t)n];

    /* free the allocated memory */
#ifdef __cplusplus
    delete[] a_seq;
#else
    free(a_seq);
#endif

    return result;
}
