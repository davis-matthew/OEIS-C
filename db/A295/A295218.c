/*
 * A295218  Number of partitions of 2*n-1 into four squares.
 * https://oeis.org/A295218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of 2*n‑1 into four squares
 *            (0 allowed). The algorithm enumerates all non‑decreasing
 *            quadruples (a,b,c,d) with a^2+b^2+c^2+d^2 = 2*n‑1
 * Range:     u8 n <= 2992, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root for unsigned 64‑bit values (works for small n). */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)  /* mid*mid > n, guard against overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Mapping from perfect squares to their square roots for 0 ≤ x ≤ 20000
   (necessary to test whether a remainder is a perfect square). */
static int root_of[20001];
static bool root_init = false;

static void init_root_map(void)
{
    if (root_init) return;
    for (int i = 0; i <= 20000; ++i) root_of[i] = -1;
    for (int i = 0; i * i <= 20000; ++i) root_of[i * i] = i;
    root_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 for safety */

    /* 2*n-1 fits in idx_t (n ≤ 10 000 → value ≤ 19 999). */
    idx_t N = 2 * n - 1;
    idx_t count = 0;

    init_root_map();

    idx_t msq = int_sqrt(N);
    for (idx_t d = 0; d <= msq; ++d) {
        idx_t d2 = d * d;
        idx_t rem1 = N - d2;
        idx_t msq_d = int_sqrt(rem1);
        if (msq_d > d) msq_d = d;

        for (idx_t c = 0; c <= msq_d; ++c) {
            idx_t c2 = c * c;
            idx_t rem2 = rem1 - c2;
            idx_t msq_dc = int_sqrt(rem2);
            if (msq_dc > c) msq_dc = c;

            for (idx_t b = 0; b <= msq_dc; ++b) {
                idx_t b2 = b * b;
                idx_t rem3 = rem2 - b2;
                int aroot = root_of[(int)rem3];
                if (aroot >= 0 && (idx_t)aroot <= b)
                    ++count;
            }
        }
    }

    return (seq_t)count;
}
