/*
 * A243290  The index of the greatest prime dividing the n-th squarefree
 *          number: a(n) = A061395(A005117(n)).
 * https://oeis.org/A243290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index of the greatest prime dividing the n‑th
 *            squarefree number. a(1) = 0 (1 has no prime divisor). For n>1 we
 *            find the n‑th squarefree number, take its largest prime factor
 *            p, and return the position of p in the ordered list of primes
 *            (index starting at 1)
 * Range:     u8 n <= 638, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/build_primes_upto.h"

/* Static state for a shared prime table. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static int primes_ready = 0;

/* Build a prime list once; limit covers all n up to at least 10000. */
static void init_prime_table(void)
{
    if (primes_ready) return;
    const idx_t LIMIT = 20000;          /* > expected squarefree numbers for n≤10000 */
    build_primes_upto(LIMIT, &prime_list, &prime_cnt);
    primes_ready = 1;
}

/* Return the largest prime factor of n (0 if n<2). */
static uint64_t largest_prime_factor_uint64(uint64_t n)
{
    if (n < 2) return 0;
    uint64_t m = n;
    uint64_t maxp = 0;
    for (size_t i = 0; i < prime_cnt &&
                      (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= m; ++i)
    {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            maxp = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) maxp = m;   /* remaining prime factor larger than sqrt(original n) */
    return maxp;
}

/* Return the 1‑based index of prime p in the prime list (0 if not found). */
static size_t prime_index(uint64_t p)
{
    if (p < 2) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if ((uint64_t)prime_list[i] == p) return i + 1;
        if ((uint64_t)prime_list[i] > p) break;
    }
    return 0;  /* should never happen for valid input */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!primes_ready) init_prime_table();

    if (n == 1)                 /* special case: 1st squarefree number is 1 */
        return (seq_t)0;

    idx_t count = 0;
    idx_t m = 0;                /* current integer being tested */
    while (count < n) {         /* find the n‑th squarefree number */
        ++m;
        if (is_squarefree((seq_t)m))
            ++count;
    }

    uint64_t lp = largest_prime_factor_uint64((uint64_t)m);
    if (lp == 0) return (seq_t)0;    /* shouldn't happen for n>1 */

    size_t idx = prime_index(lp);
    return (seq_t)idx;
}
