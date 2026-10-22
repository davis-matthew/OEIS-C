/*
 * A396896  a(n) is the first prime p > n such that the concatenation of p - n
 *          and p is prime.
 * https://oeis.org/A396896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the least prime p > n such that the decimal *
 *            concatenation of p - n and p is prime. The function searches *
 *            forwards from n+1, testing primes and the resulting
 *            concatenation * with the helper functions is_prime,
 *            concat_parts, and is_prime_big
 * Range:     u8 n <= 7, u16 n <= 64, u32 n <= 64, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"       /* bool is_prime(idx_t) */
#include "../utils/concat_parts.h"   /* static seq_t concat_parts(idx_t, idx_t) */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* We assume n >= 1; behaviour for smaller n is undefined. */
    idx_t candidate = n + 1;          /* start checking strictly above n */

    for (;;) {
        if (!is_prime(candidate)) {   /* skip non‑prime candidates */
            ++candidate;
            continue;
        }

        idx_t diff = candidate - n;   /* p - n, fits in idx_t */
        seq_t concat = concat_parts(diff, candidate); /* concatenate diff and p */

        if (is_prime_big(concat))
            return (seq_t)candidate; /* found the required prime */

        ++candidate;
    }
}
