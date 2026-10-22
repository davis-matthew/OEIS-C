/*
 * A361509  a(n) = smallest Fibonacci number F(k) such that F(k) + F(n) is a
 *          prime, or -1 if no such F(k) exists.
 * https://oeis.org/A361509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, less
 * Formula:   a(n) = smallest Fibonacci F(k) such that F(k)+F(n) is prime, or
 *            -1 if none (indices: a(0) is the first term)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib_oeis, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/fib_oeis.h"

seq_t a(idx_t n)
{
    seq_t fn = fib_oeis(n);      /* F(n) */
    seq_t f_prev = 0;            /* F(0) */
    seq_t f_curr = 1;            /* F(1) */

    /* k = 0 */
    if (isPrimeGeneric(fn + f_prev))
        return f_prev;
    /* k = 1 */
    if (isPrimeGeneric(fn + f_curr))
        return f_curr;

    /* iterate k >= 2 */
    for (idx_t k = 2; k <= 1000000u; ++k) {
        seq_t f_next = f_prev + f_curr; /* F(k) */
        f_prev = f_curr;
        f_curr = f_next;
        if (isPrimeGeneric(fn + f_curr))
            return f_curr;
    }
    /* no such F(k) within our search bound */
    return (seq_t)-1;
}
