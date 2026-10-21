/*
 * A279047  Number k of modular reductions at which the recurrence relation
 *          x(i+1) = x(0) mod x(i) terminates with x(k) = 1, where x(0) =
 *          prime(n+1), x(1) = prime(n).
 * https://oeis.org/A279047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the OEIS sequence A279047.  
 * a(n) is the number of modular reductions x(i+1) = prime(n+1) mod x(i) needed 
 * for the sequence to reach 1, starting with x(0)=prime(n+1) and x(1)=prime(n).
 * Formula (implemented by simulation): a(n) = |{i ≥ 1 : prime(n+1) mod prime(n) … mod 1 }|
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n < 1)   /* behaviour below the offset is irrelevant, but avoid UB */
        return 0;

    /* Fetch the required primes */
    seq_t p_next = (seq_t) get_prime(n + 1);   /* prime(n+1) */
    seq_t cur    = (seq_t) get_prime(n);      /* prime(n) */

    seq_t count = 0;

    /* Count modular reductions until the result is 1. */
    while (cur > (seq_t)1)
    {
        cur = p_next % cur;
        ++count;
    }

    return count;
}
