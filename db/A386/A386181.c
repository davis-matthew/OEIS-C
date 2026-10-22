/*
 * A386181  Primes having only {3, 5, 7, 8} as digits.
 * https://oeis.org/A386181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth prime whose decimal digits belong to {3,5,7,8}
 * Range:     u8 n <= 7, u16 n <= 112, u32 n <= 49, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"


static seq_t *primes = NULL;             /* array of found primes */
static size_t primes_len = 0;            /* number of primes stored */
static size_t primes_cap = 0;            /* allocated capacity */

static const unsigned int allowed_digits[4] = {3, 5, 7, 8};

static int current_len = 0;              /* number of digits processed */

/* Recursive generation of all numbers of a given length comprising only
   the allowed digits.  For each complete number it checks primality
   and, if prime, adds it to the global array. */
static void generate_rec(seq_t cur, int depth, int target)
{
    if (depth == target) {
        if (is_prime_big(cur)) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) return;          /* allocation failed – abort */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = cur;
        }
        return;
    }
    for (int i = 0; i < 4; ++i) {
        generate_rec(cur * 10 + allowed_digits[i], depth + 1, target);
    }
}

/* Ensure that the global prime array contains at least `needed` primes.
   It generates primes length by length until the required amount is
   reached. */
static void ensure_enough(size_t needed)
{
    while (primes_len < needed) {
        ++current_len;
        generate_rec(0, 0, current_len);
    }
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety – sequence is 1‑based */
    ensure_enough((size_t)n);
    return primes[n - 1];
}
