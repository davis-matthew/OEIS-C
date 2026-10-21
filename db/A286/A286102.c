/*
 * A286102  Square array A(n,k) read by antidiagonals: A(n,k) = T(lcm(n,k),
 *          gcd(n,k)), where T(n,k) is sequence A000027 considered as a
 *          two-dimensional table.
 * https://oeis.org/A286102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(lcm(i,j), gcd(i,j)) where T(n,k) is the index of the
 *            pair (n,k) in the anti‑diagonal enumeration of all positive
 *            integer pairs (n,k) with n ≥ k, i.e. T(n,k) = ((n+k-2)*(n+k-1))
 *            / 2 + n
 * Range:     u8 n <= 47, u16 n <= 718, u32 n <= 8827, u64 n <= 8827,
 *            u128 n <= 8827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* Compute the greatest common divisor of two idx_t values. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sequence a(n) definition. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* the sequence is defined for n ≥ 1 */

    /* Find the smallest s such that s*(s-1)/2 ≥ n.
       The sum s corresponds to i + j.  */
    idx_t s = (idx_t)(sqrt((double)(2 * n)) + 1);
    while (((unsigned __int128)s * (s - 1)) / 2 < n) ++s;

    /* Terms preceding the current diagonal sum. */
    unsigned __int128 pre128 = ((unsigned __int128)(s - 2) * (s - 1)) / 2;
    idx_t pre = (idx_t)pre128;

    /* Coordinates (i,j) for the given index n. */
    idx_t i = n - pre;      /* 1 ≤ i ≤ s-1 */
    idx_t j = s - i;        /* 1 ≤ j ≤ s-1 */

    /* lcm(i,j) and gcd(i,j). */
    idx_t g = gcd_idx(i, j);
    /* Compute lcm using __int128 to avoid overflow. */
    unsigned __int128 l_big = ((unsigned __int128)(i / g) * j);
    idx_t l = (idx_t)l_big;

    /* Now compute T(l,g) = ((l+g-2)*(l+g-1)) / 2 + l. */
    seq_t sum_lg = (seq_t)l + (seq_t)g;
    seq_t term1 = ((sum_lg - 2) * (sum_lg - 1)) / 2;
    seq_t result = term1 + (seq_t)l;

    return result;
}
