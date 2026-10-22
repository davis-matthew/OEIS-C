/*
 * A349483  Length of cycle reached when iterating the mapping x->
 *          n*A035116(x) on 1.
 * https://oeis.org/A349483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the cycle reached when iterating the
 *            mapping x → n*A035116(x) on 1, where A035116(x) = τ(x)², τ(x) =
 *            number of divisors of x
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_squared
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/tau_squared.h"

/* One iteration of the map x → n * τ(x)². */
static seq_t next_term(seq_t x, seq_t N)
{
    seq_t t2 = tau_squared(x);
    return N * t2;
}

/* Return the length of the cycle reached from 1 under the map. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Floyd's cycle‑finding algorithm. */
    seq_t tortoise = next_term((seq_t)1, N);
    seq_t hare = next_term(tortoise, N);

    while (tortoise != hare) {
        tortoise = next_term(tortoise, N);
        hare = next_term(next_term(hare, N), N);
    }

    /* Find the cycle length λ. */
    seq_t hare2 = next_term(tortoise, N);
    seq_t lambda = (seq_t)1;
    while (hare2 != tortoise) {
        hare2 = next_term(hare2, N);
        lambda++;
    }

    return lambda;
}
