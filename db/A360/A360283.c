/*
 * A360283  a(n) = lcm({n! * binomial(n, k) for k = 0..n}).
 * https://oeis.org/A360283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * lcm(1..n+1) / (n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A360283: a(n) = lcm({n! * binomial(n, k) | k = 0..n})   */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Computes the nth term of OEIS sequence A360283 */
seq_t a(idx_t n)
{
    /* static cache of primes up to the largest requested n+1 */
    static uint64_t *prime_array = NULL;
    static size_t prime_count = 0;
    static idx_t cached_max = 0;  /* last n for which primes were built */

    /* Build primes up to n+1 on first use or if new n exceeds cache */
    if (n > cached_max) {
        /* free previous array if it exists */
        if (prime_array != NULL)
            free(prime_array);
        /* n+1 is the limit for LCM(1..n+1) */
        build_primes_upto(n + 1, &prime_array, &prime_count);
        cached_max = n;
    }

    seq_t result = (seq_t)1;  /* identity for multiplication */

    /* Iterate each prime p <= n+1 */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_array[i];

        /* exponent of p in n!   (Legendre's formula) */
        idx_t exp_factorial = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            exp_factorial += t;
        }

        /* exponent of p in n+1 */
        idx_t exp_nplus1 = 0;
        idx_t tmp = n + 1;
        while (tmp % p == 0) {
            tmp /= p;
            exp_nplus1++;
        }

        /* floor(log_p(n+1))  – highest power of p not exceeding n+1 */
        idx_t log_p = 0;
        idx_t pow = p;
        while (pow <= n + 1) {
            log_p++;
            if (pow > (n + 1) / p)  /* prevent overflow */
                break;
            pow *= p;
        }

        /* exponent of p in the final answer */
        idx_t exp_ans = exp_factorial + log_p - exp_nplus1;
        if (exp_ans == 0)
            continue;

        seq_t factor = ipow_u64_idx(p, exp_ans);
        result = seq_mul(result, factor);
    }

    return result;
}
