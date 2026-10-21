/*
 * A254755  Left-truncatable composites: every decimal suffix is a composite
 *          number.
 * https://oeis.org/A254755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Checks left‑truncatable composites: every decimal suffix of a number is composite ( >1 and not prime ). */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Returns true iff n is composite ( >1 and not prime ). */
static inline bool is_composite(seq_t n)
{
    return (n > (seq_t)1) && !is_prime_big(n);
}

/* Returns true iff every decimal suffix of num is composite. */
static inline bool is_left_trunc_composite(seq_t num)
{
    /* Check suffixes of length 1,2,3,... .  Create powers of 10 incrementally. */
    seq_t pow10 = (seq_t)10;
    while (pow10 <= num) {
        seq_t suffix = num % pow10;   /* last digits of length log10(pow10) */
        if (!is_composite(suffix))
            return false;
        pow10 *= (seq_t)10;
    }
    /* Finally check the number itself. */
    return is_composite(num);
}

/* Return the n-th left‑truncatable composite number (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Sequence is 1‑based; return 0 for out‑of‑range input. */
        return (seq_t)0;
    }

    idx_t count = 0;
    seq_t candidate = (seq_t)4;  /* smallest composite digit */

    while (1) {
        if (is_left_trunc_composite(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
