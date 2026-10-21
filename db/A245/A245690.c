/*
 * A245690  a(n) = A245689(n) mod A053669(n).
 * https://oeis.org/A245690
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (smallest divisor of n that is > s(n)) mod s(n) where
 *            s(n) = the smallest prime that does NOT divide n. This
 *            implements the definition of OEIS A245690
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Internal prime table, lazily initialized */
static uint64_t *prime_tbl = NULL;
static size_t   prime_cnt = 0;

/* Build a list of small primes up to LIMIT (here 2000).  This is
   sufficient for all n that appear in the a(n) test suite.  */
static void init_primes(void)
{
    if (prime_tbl != NULL)
        return;                         /* already done */

    const idx_t LIMIT = 2000;           /* 2000 > any prime needed for n <= 10⁴   */
    bool *is_comp = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= (size_t)LIMIT; ++i)
        is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    for (idx_t p = 2; p * p <= LIMIT; ++p)
        if (!is_comp[(size_t)p])
            for (idx_t q = p * p; q <= LIMIT; q += p)
                is_comp[(size_t)q] = true;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[(size_t)i])
            ++cnt;

    prime_tbl = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    prime_cnt = cnt;
    size_t k = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[(size_t)i])
            prime_tbl[k++] = (uint64_t)i;

    free(is_comp);
}

/* Returns a(n) for n >= 3.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    init_primes();                 /* ensure prime list exists */

    /* 1.  Find the smallest prime not dividing n. */
    uint64_t nondiv_prime = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (n % p != 0) {          /* p is the first missing prime divisor */
            nondiv_prime = p;
            break;
        }
    }
    /* For n >= 3 such a prime always exists within our list */

    /* 2.  Find the smallest divisor of n that is > nondiv_prime. */
    idx_t best = n;                /* n itself is always a divisor */
    idx_t d;
    for (d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 > nondiv_prime && d1 < best)
                best = d1;
            if (d2 > nondiv_prime && d2 < best)
                best = d2;
        }
    }

    /* 3.  Return the residue modulo the non‑dividing prime. */
    uint64_t result = (uint64_t)best % nondiv_prime;
    return (seq_t)result;
}
