/*
 * A300623  Let b(1) = 1; for n >= 2, b(n) = n - b(t(n)) - b(n-t(n-1)) where t
 *          = A302128. a(n) = 2*b(n) - n.
 * https://oeis.org/A300623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   b(1) = 1; b(n) = n - b(t(n)) - b(n - t(n-1)) for n >= 2, where t
 *            = A302128: t(n) = t(t(n-2)) + t(n-t(n-1)), t(1)=t(2)=t(3)=1;
 *            a(n) = 2*b(n) - n
 * Range:     i8 n <= 1147, i16 n <= 28657, i32 n <= 28657, i64 n <= 28657,
 *            i128 n <= 28657, big any n
 * Validated: standard, pass, 109 terms through n = 9825 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned *A300623_T = 0;
static int *A300623_B = 0;
static idx_t A300623_cnt = 0;
static idx_t A300623_cap = 0;

static void A300623_ensure(idx_t n)
{
    if (n > A300623_cap) {
        idx_t nc = A300623_cap * 2;
        if (nc < n) nc = n;
        nc += 8;
        A300623_T = (unsigned *)realloc(A300623_T, (size_t)(nc + 1) * sizeof(unsigned));
        A300623_B = (int *)realloc(A300623_B, (size_t)(nc + 1) * sizeof(int));
        A300623_cap = nc;
    }
    for (idx_t i = A300623_cnt + 1; i <= n; i++) {
        if (i <= 3) {
            A300623_T[i] = 1;
        } else {
            A300623_T[i] = A300623_T[A300623_T[i - 2]] + A300623_T[i - A300623_T[i - 1]];
        }
        if (i == 1) {
            A300623_B[i] = 1;
        } else {
            A300623_B[i] = (int)i - A300623_B[A300623_T[i]]
                         - A300623_B[i - A300623_T[i - 1]];
        }
    }
    if (n > A300623_cnt) A300623_cnt = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A300623_ensure(n);
    return (seq_t)2 * (seq_t)A300623_B[n] - (seq_t)n;
}
