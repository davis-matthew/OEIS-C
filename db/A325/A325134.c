/*
 * A325134  a(1) = 1; a(n) = number of prime factors of n counted with
 *          multiplicity plus the largest prime index of n.
 * https://oeis.org/A325134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n counted with multiplicity +
 *            the largest prime index of n (i.e. index of the largest prime
 *            divisor)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of primes */
static idx_t   *prime_tbl   = NULL;
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

/* Extend prime table up to 'limit' (inclusive) */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Free previous prime array */
    if (prime_tbl) {
        free(prime_tbl);
        prime_tbl = NULL;
    }

    /* Allocate new prime array large enough for worst case */
    prime_tbl = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!prime_tbl)
        return; /* allocation failure – unlikely for typical limits */

    /* Simple sieve – rebuild everything up to 'limit' */
    unsigned char *is_comp = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
    if (!is_comp) {
        free(prime_tbl);
        prime_tbl = NULL;
        return;
    }
    for (idx_t i = 0; i <= limit; ++i)
        is_comp[i] = 0;

    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_tbl[prime_cnt++] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    prime_limit = limit;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;               /* a(1) = 1 */

    /* Build primes up to n (largest prime divisor ≤ n) */
    ensure_primes(n);

    uint64_t    val = (uint64_t)n;
    idx_t       omega = 0;
    uint64_t    maxp   = 0;

    /* Factor n using the prime list */
    for (size_t i = 0; i < prime_cnt && (prime_tbl[i] * prime_tbl[i] <= val); ++i) {
        idx_t p = prime_tbl[i];
        if (val % p == 0) {
            do {
                val /= p;
                ++omega;
                if (p > maxp)
                    maxp = p;
            } while (val % p == 0);
        }
    }

    /* Remaining prime factor, if any */
    if (val > 1) {
        ++omega;
        if (val > maxp)
            maxp = val;
    }

    /* Find the index (1‑based) of the largest prime divisor */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)prime_tbl[mid] < (idx_t)maxp)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t prime_idx = (idx_t)(lo + 1);   /* primes are 1‑based */

    return (seq_t)(omega + prime_idx);
}
