/*
 * A272327  Table read by antidiagonals: T(n, k) is the least i > n such that
 *          n divides i^k (n > 0, k > 0).
 * https://oeis.org/A272327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8128, u16 n <= 9989, u32 n <= 9989, u64 n <= 9989,
 *            u128 n <= 9989, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A272327.
   For a given index n, determine the coordinates (row, col) of the
   T(row, col) entry by antidiagonal enumeration.  Then compute the
   smallest integer i > row such that row divides i^col.  This equals
   ((row / B) + 1) * B where B = ∏ p^{ceil(e/power)} and
   e is the exponent of prime p in the factorisation of row. */
#include <stdint.h>
#include <stdbool.h>

/* Find the diagonal d such that (d-1)*d/2 >= n.
   Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t find_diagonal(idx_t n)
{
    idx_t low = 2, high = 2;
    /* Increase high until the condition holds */
    while (((unsigned __int128)high * (high - 1)) / 2 < n)
        high <<= 1;
    /* Binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid - 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine (row, col) from index n */
    idx_t d = find_diagonal(n);
    idx_t cum_prev = ((unsigned __int128)(d - 2) * (d - 1)) / 2;
    idx_t offset = n - cum_prev - 1;        /* zero‑based offset in this diagonal */
    idx_t row = d - 1 - offset;             /* n in the definition of T */
    idx_t col = d - row;                    /* k in the definition of T */

    /* Compute B = ∏ p^{ceil(e/k)} for prime factors of 'row' */
    seq_t B = 1;
    idx_t tmp = row;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            idx_t t = (e + col - 1) / col; /* ceil(e/col) */
            seq_t pow = 1;
            for (idx_t c = 0; c < t; ++c)
                pow *= (seq_t)p;
            B *= pow;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        idx_t e = 1;
        idx_t t = (e + col - 1) / col;
        seq_t pow = 1;
        for (idx_t c = 0; c < t; ++c)
            pow *= (seq_t)tmp;
        B *= pow;
    }

    /* Calculate the least i > row with i^col divisible by row */
    seq_t m = (seq_t)row;
    seq_t div = m / B;
    seq_t result = (div + (seq_t)1) * B;
    return result;
}
