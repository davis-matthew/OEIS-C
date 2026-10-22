/*
 * A331103  T(n,k) = -(-1)^k*k^2 mod p, where p is the n-th prime congruent to
 *          2 or 3 mod 4; triangle T(n,k), n>=1, 0<=k<=p-1, read by rows.
 * https://oeis.org/A331103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3355, u16 n <= 8361, u32 n <= 8361, u64 n <= 8361,
 *            u128 n <= 8361, big any n
 * Helpers:   build_primes_upto, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A331103 – Triangular sequence
    T(n,k) = -(-1)^k * k^2 (mod p),  p = n-th prime ≡ 2 or 3 (mod 4),
    0 ≤ k ≤ p – 1.  The rows are concatenated to give the sequence a(n).  */

/*  Required helper prototypes (provided by the build environment)  */
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Static tables for the relevant primes and cumulative row lengths  */
static uint64_t *prime_tbl   = NULL;   /* primes ≡ 2 or 3 (mod 4)   */
static uint64_t *cum_tbl     = NULL;   /* cumulative length before each row */
static size_t    prime_cnt   = 0;       /* number of stored primes   */
static size_t    prime_cap   = 0;       /* allocated capacity        */
static size_t    cum_cap     = 0;       /* allocated capacity for cum_tbl */

/*  The largest prime that has been sieved up to (for incremental sieving)  */
static uint64_t sieve_limit = 0;

/*  Cumulative length of all rows processed so far (sum of primes)  */
static uint64_t cum_total   = 0;

/*  Ensure that prime_tbl and cum_tbl contain all primes ≡2 or 3 (mod 4)
    with cumulative length at least target_n (i.e., enough rows to cover
    position target_n).  */
static void ensure_primes_up_to(uint64_t target_n)
{
    while (cum_total < target_n)
    {
        uint64_t new_limit = (sieve_limit == 0) ? 200 : sieve_limit * 2;

        /*  All primes up to new_limit  */
        uint64_t *all_primes = NULL;
        size_t    all_cnt    = 0;
        build_primes_upto(new_limit, &all_primes, &all_cnt);

        /*  Append only the new primes that satisfy p ≡ 2 or 3 (mod 4)  */
        for (size_t i = 0; i < all_cnt; ++i)
        {
            uint64_t p = all_primes[i];
            if (p <= sieve_limit)            /* already processed */
                continue;
            if ((p & 3) == 2 || (p & 3) == 3)
            {
                /*  Grow tables if necessary  */
                ensure_capacity(prime_cnt + 1, &prime_cap,
                                (void **)&prime_tbl, sizeof *prime_tbl);
                ensure_capacity(prime_cnt + 1, &cum_cap,
                                (void **)&cum_tbl,   sizeof *cum_tbl);

                /*  Store prime and its cumulative length before this row  */
                prime_tbl[prime_cnt] = p;
                cum_tbl[prime_cnt]   = cum_total;
                cum_total           += p;
                ++prime_cnt;
            }
        }

        free(all_primes);
        sieve_limit = new_limit;   /* next time we’ll start from here   */
    }
}

/*  OEIS entry function  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* out of range – not expected */

    /*  Make sure we have enough rows to cover position n  */
    ensure_primes_up_to((uint64_t)n);

    /*  Locate the row containing position n  */
    size_t row = 0;
    while (row < prime_cnt &&
           cum_tbl[row] + prime_tbl[row] < (uint64_t)n)
    {
        ++row;
    }

    /*  Compute k and the value modulo p  */
    uint64_t p  = prime_tbl[row];
    uint64_t k  = (uint64_t)n - cum_tbl[row] - 1;          /* 0‑based k */
    uint64_t ksq= (k * k) % p;

    uint64_t v;
    if ((k & 1) == 0)                       /* k even  →  negative sign   */
    {
        v = (ksq == 0) ? 0 : p - ksq;
    }
    else                                     /* k odd   →  positive sign   */
    {
        v = ksq;
    }

    return (seq_t)v;
}
