/*
 * A386020  Primes having only {0, 1, 2, 7} as digits.
 * https://oeis.org/A386020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 113, u32 n <= 46, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search over integers whose decimal digits belong to {0,1,2,7},
   test each for primality, and return the nth such prime.  */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static const seq_t allowed_digits[4] = {0u, 1u, 2u, 7u};

/* Recursive generator: visit all numbers of a fixed length 'len' in
   numeric order (digits are tried in increasing order).  Whenever a
   prime is found it is appended to global array 'primes'.  */
static void dfs_generate(int pos, int len, seq_t current,
                         seq_t **primes, size_t *cap, idx_t *cnt)
{
    if (pos > len) {                     /* number complete */
        if (is_prime_big(current)) {
            (*cnt)++;
            grow_seq(primes, cap, (size_t)*cnt);
            (*primes)[*cnt - 1] = current;
        }
        return;
    }

    for (int d = 0; d < 4; ++d) {
        if (pos == 1 && allowed_digits[d] == 0)
            continue;                    /* no leading zero */
        seq_t next = current * 10 + allowed_digits[d];
        dfs_generate(pos + 1, len, next, primes, cap, cnt);
    }
}

/* Public sequence function requested by the build system. */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;          /* collected primes so far   */
    static size_t primes_cap = 0;
    static idx_t prime_count = 0;         /* number of primes collected */
    static int current_len = 1;           /* current length being generated */

    /* Generate primes until we have at least n of them. */
    while (prime_count < n) {
        dfs_generate(1, current_len, (seq_t)0,
                     &primes, &primes_cap, &prime_count);
        ++current_len;
    }
    return primes[n - 1];
}
