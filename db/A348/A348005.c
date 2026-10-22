/*
 * A348005  Positive even integers with an even number of even divisors.
 * https://oeis.org/A348005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formulas: a(n) = 2 * m where m is the n-th positive integer that does
   not have the form 2^(odd) * (odd square).  Equivalently,
   a(n) = 2 * A000037(n) as given in OEIS.  Here we generate the
   required numbers by testing each even integer for the exclusion
   criterion.  */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer.
   Uses only division to avoid overflow. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        /* mid*mid <= n  <=> mid <= n / mid (mid != 0) */
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t count = 0;      /* number of terms found so far   */
    idx_t i = 1;          /* index of the next even integer */

    while (true) {
        seq_t candidate = (seq_t)2 * (seq_t)i; /* even number */
        /* factor out powers of two */
        idx_t temp = (idx_t)candidate;
        idx_t k = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            k++;
        }
        /* temp is now the odd part m */
        idx_t sqrt_m = isqrt_u64(temp);
        bool is_square = (sqrt_m * sqrt_m == temp);
        /* exclude if m is a perfect square and k is odd */
        if (!(is_square && (k & 1))) {
            count++;
            if (count == n)
                return candidate;
        }
        i++;
    }
}
