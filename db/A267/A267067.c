/*
 * A267067  Primes p such that mu(p-2) = 1; that is, p-2 is squarefree and has
 *          an even number of prime factors, where mu is the Moebius function
 *          (A008683).
 * https://oeis.org/A267067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that mu(p-2) = 1 * where mu is the
 *            Möbius function. * This implementation searches candidates
 *            incrementally, * checks primality and the Möbius condition, and
 *            caches * found primes so that any order of calls is handled
 * Range:     u8 n <= 21, u16 n <= 2444, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9307 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    /* Cache of primes that satisfy the condition */
    static seq_t *valid = NULL;
    static size_t val_cnt = 0;      /* number of cached primes */
    static size_t val_cap = 0;      /* allocated capacity */

    /* Next odd integer to test for primeness (start at 3) */
    static uint64_t next_candidate = 3;

    if (n == 0) return 0;          /* undefined for offset 0, safe guard */

    /* Generate primes until we have at least n valid ones */
    while (val_cnt < n) {
        /* Test current candidate */
        if (is_prime(next_candidate)) {
            /* p-2 > 0 and check Möbius */
            if (mu_of(next_candidate - 2) == 1) {
                /* Expand cache if necessary */
                if (val_cnt == val_cap) {
                    val_cap = val_cap ? val_cap * 2 : 4;
                    seq_t *tmp = mem_resize(seq_t, valid, val_cap);
                    if (!tmp) abort();          /* allocation failure */
                    valid = tmp;
                }
                valid[val_cnt++] = (seq_t)next_candidate;
            }
        }
        /* Advance to next odd number */
        next_candidate += 2;
    }

    return valid[n - 1];
}
