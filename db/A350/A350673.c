/*
 * A350673  Square array read by antidiagonals T(n, k), n, k >= 0; T(n, k) is
 *          the sum of the elements of the intersection of the multisets of
 *          positive squares summing to n and k, respectively, obtained by the
 *          greedy algorithm.
 * https://oeis.org/A350673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A350673
    a(n) = T(i,j), where (i,j) is the nth element read
    by antidiagonals of the square array.
    T(i,j) is the sum of all positive squares that
    appear in both greedy representations of i and j.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- integer square‑root for uint64_t (no floating point) ---------- */
static inline idx_t uint64_isqrt(idx_t n)
{
    /* binary search on [0, n] for floor(sqrt(n)) */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = ( __uint128_t)mid * mid;
        if (sq == n)
            return mid;
        else if (sq < n)
            low = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* ---------- find diagonal number k given global index n ---------- */
static inline idx_t diag_from_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* exponential search for an upper bound */
    while (((__uint128_t)high * (high + 1)) / 2 <= n)
        high <<= 1;
    /* binary search in [low, high) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((__uint128_t)mid * (mid + 1)) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;               /* k satisfies k*(k+1)/2 <= n */
}

/* ---------- greedy representation intersection sum ---------- */
static seq_t greedy_intersection(idx_t a, idx_t b)
{
    /* maximum value needed for the counting array */
    idx_t max_ab = (a > b) ? a : b;
    idx_t max_sqrt = uint64_isqrt(max_ab);
    size_t size = (size_t)max_sqrt + 1;
    int *cnt_a = (int *)calloc(size, sizeof(int));
    int *cnt_b = (int *)calloc(size, sizeof(int));
    if (!cnt_a || !cnt_b) {
        /* Out of memory – abort (should not happen for the intended range). */
        exit(EXIT_FAILURE);
    }

    /* count squares in a */
    idx_t rem = a;
    while (rem > 0) {
        idx_t s = uint64_isqrt(rem);
        ++cnt_a[s];
        rem -= s * s;
    }

    /* count squares in b */
    rem = b;
    while (rem > 0) {
        idx_t s = uint64_isqrt(rem);
        ++cnt_b[s];
        rem -= s * s;
    }

    /* accumulate intersection */
    seq_t sum = 0;
    for (idx_t s = 1; s <= max_sqrt; ++s) {
        int ca = cnt_a[s];
        int cb = cnt_b[s];
        if (ca && cb) {
            seq_t sq = (seq_t)s * (seq_t)s;
            seq_t mult = (ca < cb) ? (seq_t)ca : (seq_t)cb;
            sum += sq * mult;
        }
    }

    free(cnt_a);
    free(cnt_b);
    return sum;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* obtain diagonal index k and offset within that diagonal */
    idx_t k = diag_from_index(n);
    idx_t start = (idx_t)k * (k + 1) / 2;
    idx_t offset = n - start;
    idx_t i = offset;
    idx_t j = k - offset;

    return greedy_intersection(i, j);
}
