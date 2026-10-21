/*
 * A252933  Number of n X 3 nonnegative integer arrays with upper left 0 and
 *          every value within 3 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A252933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (160/3)*n^3 - 548*n^2 + (6071/3)*n - 2591 (for n>4)
 *            Recurrence for n>8: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) -
 *            a(n-4) Initial values: a(1)=4, a(2)=13, a(3)=44, a(4)=153,
 *            a(5)=494, a(6)=1343, a(7)=3016, a(8)=5833
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[9] = {0,4,13,44,153,494,1343,3016,5833};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* valid for n>=1 only */

    static seq_t *seq = NULL;
    static size_t cap = 0;
    size_t N = (size_t)n;

    if (cap == 0) {
        size_t need = (N > 8) ? N : 8;
        seq = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        if (!seq) return 0;
        cap = need;
        for (size_t i = 1; i <= 8 && i <= cap; ++i)
            seq[i] = init_vals[i];
        for (size_t i = 9; i <= cap; ++i)
            seq[i] = 4 * seq[i-1] - 6 * seq[i-2] + 4 * seq[i-3] - seq[i-4];
    }
    else if (cap < N) {
        size_t old = cap;
        seq = (seq_t *)realloc(seq, (N + 1) * sizeof(seq_t));
        if (!seq) return 0;
        cap = N;
        for (size_t i = old + 1; i <= cap; ++i) {
            if (i <= 8)
                seq[i] = init_vals[i];
            else
                seq[i] = 4 * seq[i-1] - 6 * seq[i-2] + 4 * seq[i-3] - seq[i-4];
        }
    }

    return seq[N];
}
