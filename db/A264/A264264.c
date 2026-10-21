/*
 * A264264  The length of the shortest nontrivial integral cevian of an
 *          isosceles triangle, with base of length 1 and legs of length n,
 *          that divides the base into two integral parts.
 * https://oeis.org/A264264
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{ (d1 + d2)/4 : d1*d2 = 4n²-1, d1,d2 odd, L > n } The
 *            shortest nontrivial integral cevian length for the isosceles
 *            triangle with legs n and base 1 is obtained by factoring 4n²-1
 *            into odd factors d1 ≤ d2. For each such pair the cevian length L
 *            is (d1+d2)/4. The trivial cevian (length n)...
 * Range:     u8 n <= 8, u16 n <= 260, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0. */
    if (n < 2) return 0;

    /* Compute M = 4*n*n - 1 (odd). */
    idx_t M = 4 * n * n - 1;

    seq_t best = 0;
    int found = 0;

    /* Iterate over odd divisors i up to sqrt(M). */
    for (idx_t i = 1; i * i <= M; i += 2) {
        if (M % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = M / i;

        /* The sum must be divisible by 4 to yield an integer L. */
        if (((d1 + d2) & 3) != 0) continue;

        seq_t L = ((seq_t)d1 + (seq_t)d2) / 4;

        if (L <= n) continue;          /* skip the trivial cevian (length n) */
        if (!found || L < best) {
            best = L;
            found = 1;
        }
    }

    return best;
}
