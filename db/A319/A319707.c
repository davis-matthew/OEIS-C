/*
 * A319707  Filter sequence which records for primes their residue modulo 6,
 *          and for all other numbers assigns a unique number.
 * https://oeis.org/A319707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n = 2; 3 if n = 3; * a(n) = 5 if n % 6 = 5 (prime);
 *            * a(n) = 7 if n % 6 = 1 (prime); * otherwise a(n) is the
 *            smallest unused positive integer * that has not yet appeared
 *            (skipping 2,3,5,7). * This is the restricted‑growth transform of
 *            * f(n) = A010875(n) when n is prime, * f(n) = -n o...
 * Range:     u8 n <= 317, u16 n <= 9434, u32 n <= 9434, u64 n <= 9434,
 *            u128 n <= 9434, big any n
 * Helpers:   alloc_seq_array, grow_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The implementation caches computed values so that calls to a(n)
 *   are O(1) after the first required expansion.  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t computed = 0;       /* largest index computed so far */
    static seq_t next_unused = 1;    /* next candidate for composites */

    if (n == 0) {
        /* Not part of the OEIS range; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Grow the cache if necessary */
    if (n > cache_cap) {
        grow_seq_array(&cache, &cache_cap, n);
    }

    /* Compute new entries */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (is_prime(i)) {
            /* Prime handling */
            if (i == 2) {
                val = 2;
            } else if (i == 3) {
                val = 3;
            } else if (i % 6 == 5) {
                val = 5;
            } else { /* i % 6 == 1 for primes >=5 */
                val = 7;
            }
            /* Primes never advance next_unused */
        } else {
            /* Composite: assign the next unused integer */
            val = next_unused;
            /* Advance next_unused, skipping the reserved prime values */
            do {
                ++next_unused;
            } while (next_unused == 2 ||
                     next_unused == 3 ||
                     next_unused == 5 ||
                     next_unused == 7);
        }
        cache[i - 1] = val;
    }

    computed = n;
    return cache[n - 1];
}
