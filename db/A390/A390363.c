/*
 * A390363  Sum of the two greatest prime indices of n, or 0 if n is 1 or a
 *          prime number.
 * https://oeis.org/A390363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   Sum of the two greatest prime indices of n, or 0 if n is 1 or a
 *            prime number
 * Range:     u8 n <= 3225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;      /* array of primes (1‑based indices) */
static size_t prime_cnt   = 0;         /* number of primes stored     */
static idx_t  prime_limit = 0;        /* maximum value for which primes are pre‑computed */

/* Ensure primes are available up to the given limit.  Rebuild the list if needed. */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit) return;

    /* free previous list if any */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    build_primes(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Binary search for the 1‑based index of prime p in prime_list. */
static int prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else if (prime_list[mid] > p)
            hi = mid;
        else
            return (int)(mid + 1);   /* 1‑based index */
    }
    return -1; /* not found */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* a(1) = 0 */

    /* Ensure we have primes up to n */
    ensure_primes(n);

    if (is_prime(n)) return 0;     /* primes yield 0 */

    seq_t max1 = 0, max2 = 0;      /* keep two largest prime indices */
    idx_t temp = n;

    /* Divide by small primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > temp / p) break;          /* p*p > temp */
        while (temp % p == 0) {
            seq_t idx = (seq_t)(i + 1);    /* 1‑based prime index */
            if (idx > max1) { max2 = max1; max1 = idx; }
            else if (idx > max2) { max2 = idx; }
            temp /= p;
        }
    }

    /* Remaining factor (if >1) is prime */
    if (temp > 1) {
        int idxp = prime_index((uint64_t)temp);
        if (idxp > 0) {
            seq_t idx = (seq_t)idxp;
            if (idx > max1) { max2 = max1; max1 = idx; }
            else if (idx > max2) { max2 = idx; }
        }
    }

    return max1 + max2;
}
