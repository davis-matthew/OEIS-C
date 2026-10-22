/*
 * A317768  Number of nX3 0..1 arrays with every element unequal to 0, 1 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-4) - 2*a(n-5) - 11*a(n-8) - 3*a(n-9) +
 *            6*a(n-12) + 4*a(n-13) - a(n-16) - a(n-17) for n > 23 Initial
 *            terms (1‑based index): a(1)=3, a(2)=4, a(3)=5, a(4)=11, a(5)=16,
 *            a(6)=28, a(7)=44, a(8)=74, a(9)=122, a(10)=209, a(11)=344,
 *            a(12)=584, a(13)=981, a(14)=1670, a(15)=2806, a(16)...
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* helper array of the first 24 terms, indexed from 1; index 0 unused */
static const seq_t _init[25] = {
    0ULL, /* unused */
    3ULL, 4ULL, 5ULL, 11ULL, 16ULL, 28ULL,
    44ULL, 74ULL, 122ULL, 209ULL, 344ULL,
    584ULL, 981ULL, 1670ULL, 2806ULL, 4758ULL,
    8047ULL, 13665ULL, 23111ULL, 39195ULL,
    66419ULL, 112700ULL, 190989ULL, 323943ULL
};

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }
    if (n <= 24) {
        return _init[n];
    }

    /* allocate array sized n+1 (0..n) */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) {
        /* allocation failed – return 0 (unlikely for supported n) */
        return (seq_t)0;
    }

    /* copy known initial terms */
    for (idx_t i = 1; i <= 24; ++i) {
        arr[i] = _init[i];
    }

    /* compute further terms using the recurrence */
    for (idx_t i = 25; i <= n; ++i) {
        seq_t pos = arr[i - 1]
                  + (seq_t)6 * arr[i - 4]
                  + (seq_t)6 * arr[i - 12]
                  + (seq_t)4 * arr[i - 13];
        seq_t neg = (seq_t)2 * arr[i - 5]
                  + (seq_t)11 * arr[i - 8]
                  + (seq_t)3 * arr[i - 9]
                  + arr[i - 16]
                  + arr[i - 17];
        arr[i] = pos - neg;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
