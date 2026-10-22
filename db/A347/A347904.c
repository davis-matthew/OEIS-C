/*
 * A347904  Array read by antidiagonals, m, n >= 1: T(m,n) is the first prime
 *          (after the two initial terms) in the Fibonacci-like sequence with
 *          initial terms m and n, or 0 if no such prime exists.
 * https://oeis.org/A347904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n). T(m,n) is the first prime that appears after the
 *            two initial terms in the Fibonacci‑like sequence with initial
 *            terms m and n (read by antidiagonals, m,n≥1). If m and n are not
 *            coprime or if no prime ever appears, T(m,n)=0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   antidiagonal_coords_calc, gcd, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/antidiagonal_coords_calc.h"

static seq_t compute_T(seq_t m, seq_t n)
{
    if (gcd(m, n) != (seq_t)1)
        return 0;  /* initial terms not coprime → no prime */

    seq_t a = m, b = n;
    /* Search for the first prime in the generated sequence.  The number of
       terms needed is extremely small in practice, but we set a generous
       upper bound to avoid infinite loops in pathological cases. */
    for (size_t iter = 0; iter < 20000; ++iter) {
        seq_t s = a + b;
        if (isPrimeGeneric(s))
            return s;
        a = b;
        b = s;
    }
    return 0;  /* did not find a prime within the bound */
}

seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset is 1 */
        return 0;

    idx_t i, j;
    antidiagonal_coords_calc(n, &i, &j);
    seq_t m = (seq_t)i;
    seq_t k = (seq_t)j;
    return compute_T(m, k);
}
