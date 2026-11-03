/*
 * A391735  Square array read by descending antidiagonals: A(n, k) is the k-th
 *          odd number i such that i divides A048720(n,i), where A048720 is
 *          carryless base-2 multiplication.
 * https://oeis.org/A391735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is obtained by reading the square array A(n,k) (k‑th odd
 *            number i * such that i divides the carryless base‑2 product of n
 *            and i) by descending * antidiagonals. The code computes a(n) for
 *            any n ≥ 1. * * Sequence definition from OEIS A391735
 * Range:     u8 n <= 204, u16 n <= 3500, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 107 terms through n = 8981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Locate the diagonal (sum n+k) that contains the given index idx (>=1).
 * The cumulative number of terms up to diagonal s is s*(s-1)/2.
 * We find the smallest s with s*(s-1)/2 ≥ idx. */
static idx_t diagonal_from_index(idx_t idx)
{
    idx_t lo = 2, hi = 2;
    while ((unsigned __int128)hi * (hi - 1) / 2 < idx)
        hi <<= 1;                     /* exponential search for upper bound */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= idx)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Compute the carryless base‑2 product of odd_n and candidate.
 * odd_n is the odd part of the row index (all 2‑powers removed).
 * candidate ranges over odd numbers. */
static seq_t carryless_product(idx_t odd_n, seq_t candidate)
{
    seq_t prod = 0;
    idx_t bits  = odd_n;
    int   shift = 0;
    while (bits) {
        if (bits & 1)
            prod ^= candidate << shift;
        bits >>= 1;
        ++shift;
    }
    return prod;
}

/* Test whether candidate divides carryless_product(odd_n, candidate). */
static int divides_carryless(idx_t odd_n, seq_t candidate)
{
    seq_t prod  = carryless_product(odd_n, candidate);
    return (prod % candidate) == 0;
}

/* Return the k‑th odd divisor (k≥1) of odd_n. */
static seq_t kth_odd_divisor(idx_t odd_n, idx_t k)
{
    seq_t candidate = 1;  /* first odd candidate */
    idx_t found     = 0;

    while (found < k) {
        if (divides_carryless(odd_n, candidate)) {
            ++found;
            if (found == k)
                return candidate;
        }
        candidate += 2;   /* next odd number */
    }
    /* Should never be reached */
    return candidate;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal and position within that diagonal */
    idx_t s = diagonal_from_index(n);
    idx_t prev = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t pos = n - prev;                       /* 1‑based position in diagonal */

    idx_t row_n = pos;             /* row index (n coordinate) */
    idx_t row_k = s - pos;         /* column index (k coordinate) */

    /* Reduce row_n by removing all powers of two (A(2·m,k)=A(m,k)) */
    idx_t odd_n = row_n;
    while ((odd_n & 1) == 0)
        odd_n >>= 1;

    return kth_odd_divisor(odd_n, row_k);
}
