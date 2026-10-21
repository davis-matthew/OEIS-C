/*
 * A277472  a(n) = (-i)^n * Integral_{x>=0} H_n(i*x) * exp(-x), where H_n(x)
 *          is n-th Hermite polynomial, i=sqrt(-1).
 * https://oeis.org/A277472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{⌊n/2⌋} 2^{n-2k} / k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           equivalently, a(n) = Σ_{k=0}^{⌊n/2⌋} (n! / k!) * 2^{n-2k} */
/*           with the recurrence T_0 = n! * 2^n, T_k = T_{k-1} / (4k) */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* T_0 = n! * 2^n */
    seq_t term = factorial(n);
    term = term * pow2(n);

    seq_t sum = term;

    idx_t max_k = n / 2;
    for (idx_t k = 1; k <= max_k; ++k) {
        /* T_k = T_{k-1} / (4k) ; division is exact */
        term = term / ( (seq_t)(4 * k) );
        sum = sum + term;
    }
    return sum;
}
