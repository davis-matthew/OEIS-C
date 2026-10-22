/*
 * A373197  Sum of all squarefree numbers from prime(n) to prime(n+1) - 1.
 * https://oeis.org/A373197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑{k = prime(n) … prime(n+1)-1, k is squarefree} k
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helpers for primes and squarefree test */
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/is_squarefree.h" /* bool is_squarefree(seq_t x); */

/* Sum of all squarefree numbers from prime(n) to prime(n+1)-1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1, safe fallback */

    /* Obtain the required consecutive primes */
    uint32_t p_start = get_prime(n);
    uint32_t p_next  = get_prime(n + 1);

    seq_t sum = 0;
    for (uint32_t x = p_start; x < p_next; ++x) {
        if (is_squarefree((seq_t)x))
            sum += (seq_t)x;
    }
    return sum;
}
