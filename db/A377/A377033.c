/*
 * A377033  Array read by antidiagonals downward where A(n,k) is the n-th term
 *          of the k-th differences of the composite numbers (A002808).
 * https://oeis.org/A377033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Σ_{k=0..j} (-1)^(j-k) * C(j,k) * composite[i+k] where
 *            (i,j) are the antidiagonal coordinates of n and composite[k] is
 *            the k‑th composite number (A002808)
 * Range:     i8 n <= 43, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* cache of composite numbers A002808(k) for 0 ≤ k ≤ current maximum */
static seq_t *comps = NULL;
static idx_t compsSize = 0;   /* number of composites stored */
static idx_t compsCap  = 0;   /* capacity of the above array */

/* ensure that composites[0 … need] exist */
static void ensure_composites(idx_t need)
{
    if (need < compsSize)
        return;
    if (compsCap < need + 1) {
        idx_t newCap = compsCap ? compsCap : 8;
        while (newCap <= need)
            newCap <<= 1;
        comps = (seq_t*)realloc(comps, newCap * sizeof(seq_t));
        compsCap = newCap;
    }

    seq_t val;
    if (compsSize == 0)
        val = (seq_t)4;          /* first composite number */
    else
        val = comps[compsSize - 1] + ((seq_t)1); /* start searching above last found */

    for (idx_t i = compsSize; i <= need; ++i) {
        while (is_prime_small(val))
            ++val;                /* skip primes */
        comps[i] = val;
        ++val;
    }
    compsSize = need + 1;
}

/* multiplicative binomial coefficient C(n,k) for integer n,k ≥ 0 */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n + 1 - i);
        result = result / (seq_t)i;          /* division is exact */
    }
    return result;
}

/* find largest s such that s*(s+1)/2 ≤ n  (integer arithmetic) */
static idx_t find_diagonal(idx_t n)
{
    idx_t low = 0, high = 1;
    while (true) {
        __int128 tri = (__int128)high * (high + 1) / 2;
        if (tri > n)
            break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* determine (i,j) coordinates from antidiagonal order */
    idx_t s = find_diagonal(n);
    idx_t triPrev = s * (s + 1) / 2;   /* n of the first element in diag s */
    idx_t offset = n - triPrev;
    idx_t i = s - offset;      /* row index (downward) */
    idx_t j = offset;          /* column index (difference order) */

    /* need composites up to i+j (k runs 0..j) */
    ensure_composites(i + j);

    /* compute A(i,j) = Σ_{k=0..j} (-1)^{j-k} C(j,k) * composite[i+k] */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= j; ++k) {
        seq_t term = comps[i + k];
        term = term * binomial(j, k);
        if ((j - k) & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
