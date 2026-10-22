/*
 * A384170  a(n) = 3^n * n! * binomial(5*n/3,n) * Sum_{k=1..n} 1/(2*n+3*k).
 * https://oeis.org/A384170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * n! * binomial(5*n/3,n) * Sum_{k=1..n} 1/(2*n+3*k)
 *            Simplified integer form: let A = Π_{i=0}^{n-1} (5*n-3*i) Then
 *            a(n) = Σ_{k=1}^{n} A / (2*n+3*k) (exact division in each term)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 1 – only defined for n ≥ 1, but guard against 0 */
    if (n == 0) return 0;

    /* If seq_t fits in 64 bits, use __int128 to avoid overflow in the
       intermediate product and the sum.  For larger seq_t types (arbitrary
       precision) we compute directly with seq_t operations.  The compiler
       will eliminate the dead branch. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 A = 1;
        for (idx_t i = 0; i < n; ++i) {
            unsigned __int128 fac = 5ull * n - 3ull * i;
            A *= fac;
        }
        unsigned __int128 sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            unsigned __int128 denom = 2ull * n + 3ull * k;
            sum += A / denom;
        }
        return (seq_t)sum;
    } else {
        seq_t A = 1;
        for (idx_t i = 0; i < n; ++i) {
            seq_t fac = (seq_t)(5 * n - 3 * i);
            A *= fac;
        }
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t denom = (seq_t)(2 * n + 3 * k);
            sum += A / denom;
        }
        return sum;
    }
}
