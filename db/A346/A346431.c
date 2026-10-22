/*
 * A346431  Primes p such that A007663(i) is divisible by Product_{k=1..7}
 *          A343763(k), where i is the index of p in A000040.
 * https://oeis.org/A346431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A346431: prime p satisfies that (2^(p‑1)‑1)/p is divisible by
 * the product 3·7·79·2731·8191·121369·22366891.  The function returns the
 * n‑th such prime (a(1) = 157).
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static const idx_t primes_to_check[7] = {
    3ULL, 7ULL, 79ULL, 2731ULL, 8191ULL, 121369ULL, 22366891ULL
};

/* test whether a prime p satisfies the condition */
static bool satisfies(idx_t p)
{
    const seq_t two = (seq_t)2;
    for (idx_t k = 0; k < 7; ++k) {
        /* modulus = p * primes_to_check[k] */
        seq_t modulus = (seq_t)p * (seq_t)primes_to_check[k];
        if (mod_pow(two, p - 1, modulus) != (seq_t)1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1, so a(0) is undefined; return 0 */
        return 0;

    idx_t found = 0;          /* count of primes satisfying the condition */
    idx_t candidate = 2;      /* current number to test */
    idx_t prime_index = 0;    /* position of candidate in the prime list */

    while (true) {
        if (is_prime(candidate)) {
            ++prime_index;
            if (satisfies(candidate)) {
                ++found;
                if (found == n)
                    return (seq_t)candidate;
            }
        }

        /* advance to next candidate prime */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;   /* skip even numbers */
    }
}
