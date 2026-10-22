/*
 * A386163  Primes having only {2, 5, 7, 9} as digits.
 * https://oeis.org/A386163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 103, u32 n <= 49, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal digits are only in {2,5,7,9}. */

/* The code generates all numbers with this digit set in increasing
   numeric order, tests each for primality (using the supplied
   is_prime_big routine), stores the primes, and returns the nth such
   prime.  It memoises the result so repeated calls with arbitrary
   indices are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *primes = NULL;           /* array holding found primes */
static size_t primes_count = 0;         /* number of primes stored */
static size_t primes_cap = 0;           /* allocated capacity */
static idx_t max_len_generated = 0;    /* last length fully processed */

static void ensure_primes_cap(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *newp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!newp) { abort(); }           /* allocation failure */
        primes = newp;
        primes_cap = new_cap;
    }
}

/* Recursive generator for numbers of a fixed length.
   `len`    – desired length
   `pos`    – current position (0 .. len)
   `cur`    – value built so far */
static void gen_len(idx_t len, idx_t pos, seq_t cur)
{
    if (pos == len) {
        if (is_prime_big(cur)) {
            ensure_primes_cap(primes_count + 1);
            primes[primes_count++] = cur;
        }
        return;
    }
    /* digits in ascending order to keep numeric order */
    static const seq_t digs[4] = { (seq_t)2, (seq_t)5, (seq_t)7, (seq_t)9 };
    for (int i = 0; i < 4; ++i) {
        gen_len(len, pos + 1, cur * (seq_t)10 + digs[i]);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for 0, but safe return */

    if (primes_count >= n)       /* already cached */
        return primes[n - 1];

    /* Generate primes until we reach at least n */
    for (idx_t len = max_len_generated + 1; primes_count < n; ++len) {
        gen_len(len, 0, 0);
        max_len_generated = len;
    }
    return primes[n - 1];
}
