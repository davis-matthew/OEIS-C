/*
 * A359514  Number of compositions (ordered partitions) of n into at most 2
 *          positive Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A359514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2} A121548(n,k) (number of compositions of n
 *            into at most 2 positive Fibonacci numbers, with a single type of
 *            1)
 * Range:     u8 n <= 9932, u16 n <= 9932, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty composition) */
    if (n == 0)
        return 1;

    /* Generate Fibonacci numbers 1,2,3,5,8,… up to n.
       The count is small (≤ 20 for n ≤ 10000), so a static array is fine. */
    idx_t fibs[64];
    size_t len = 0;

    fibs[len++] = 1;
    if (n >= 2)
        fibs[len++] = 2;

    idx_t f_prev = 1;
    idx_t f_curr = 2;
    while (1) {
        idx_t f_next = f_prev + f_curr;
        if (f_next > n)
            break;
        fibs[len++] = f_next;
        f_prev = f_curr;
        f_curr = f_next;
    }

    seq_t ans = 0;

    /* Single‑part compositions: n itself is a Fibonacci number. */
    for (size_t i = 0; i < len; ++i) {
        if (fibs[i] == n) {
            ++ans;
            break;
        }
    }

    /* Two‑part compositions: ordered pairs (fi, fj) with fi + fj = n. */
    for (size_t i = 0; i < len; ++i) {
        idx_t fi = fibs[i];
        if (fi > n)
            break;
        idx_t rem = n - fi;
        for (size_t j = 0; j < len; ++j) {
            if (fibs[j] == rem) {
                ++ans;
                break;
            }
        }
    }

    return ans;
}
