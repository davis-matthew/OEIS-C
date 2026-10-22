/*
 * A377615  a(n) is the number of iterations of x -> 2*x + 7 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x → 2*x + 7 until the number * of
 *            primes seen equals the number of composites seen, starting with
 *            prime(n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* safety for indices below 1 */

    seq_t x = (seq_t)get_prime(n);            /* prime(n) */
    seq_t prime_cnt  = 1;
    seq_t comp_cnt   = 0;
    seq_t steps      = 0;

    while (prime_cnt != comp_cnt) {
        x = (seq_t)2 * x + (seq_t)7;
        steps++;
        if (is_prime_big(x))
            prime_cnt++;
        else
            comp_cnt++;
    }
    return steps;
}
