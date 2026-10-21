/*
 * A280946  Numbers n such that n and number of proper divisors (A032741) of n
 *          are relatively prime and n is a nonprime (A018252).
 * https://oeis.org/A280946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that gcd(n, d(n)) = 1 and n is nonprime,
   where d(n) counts the proper divisors of n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state for on‑demand sieving and result storage */
static idx_t      sieve_limit = 0;   /* largest index ever sieved          */
static uint32_t  *spf = NULL;       /* smallest prime factor for each n   */
static bool      *isPrime = NULL;   /* prime flag for each n              */
static seq_t     *results = NULL;   /* sequence terms collected so far    */
static size_t     res_len   = 0;     /* number of terms stored             */
static size_t     res_cap   = 0;     /* capacity of the results array      */
static idx_t      last_checked = 0;  /* last number already processed      */

/* Euclidean GCD for idx_t values.  Works for any integral type. */
static inline idx_t
gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure spf[] and isPrime[] exist up to new_limit (inclusive).  */
static void
ensure_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;

    /* Free old arrays if any */
    free(spf);
    free(isPrime);

    /* Allocate new arrays; cast explicitly for C++ compatibility. */
    spf = (uint32_t*)calloc((size_t)new_limit + 1U, sizeof(uint32_t));
    isPrime = (bool*)calloc((size_t)new_limit + 1U, sizeof(bool));
    if (!spf || !isPrime)
        exit(1);  /* failure to allocate */

    /* Simple sieve of Eratosthenes to fill spf[] and isPrime[] */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            isPrime[i] = true;
            /* Mark composites */
            if (i <= new_limit / i) {         /* avoid overflow for i*i */
                for (idx_t j = i * i; j <= new_limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                    isPrime[j] = false;
                }
            }
        }
    }

    sieve_limit = new_limit;
    /* last_checked should not exceed the new limit */
    if (last_checked > new_limit)
        last_checked = new_limit;
}

/* Grow the result buffer to hold at least needed slots. */
static void
ensure_capacity_result(size_t needed)
{
    if (needed <= res_cap)
        return;
    size_t new_cap = res_cap ? res_cap * 2 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    results = tmp;
    res_cap = new_cap;
}

/* Return the proper divisor count of x (excluding x itself). */
static inline uint32_t
proper_divisor_count(idx_t x)
{
    if (x <= 1)
        return 0;
    uint32_t prod = 1;
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        prod *= (exp + 1U);
    }
    return prod - 1U;
}

/* Extend the answer set up to new_limit (inclusive). */
static void
generate_up_to(idx_t new_limit)
{
    ensure_sieve(new_limit);
    for (idx_t i = last_checked + 1; i <= new_limit; ++i) {
        if (!isPrime[i]) {                     /* nonprime only */
            uint32_t pd = proper_divisor_count(i);
            if (gcd_idx(i, (idx_t)pd) == 1) {
                ensure_capacity_result(res_len + 1);
                results[res_len++] = (seq_t)i;
            }
        }
    }
    last_checked = new_limit;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but safe */

    while (res_len < n) {
        idx_t next_limit = (sieve_limit == 0) ? 100 : sieve_limit * 2;
        /* Heuristic overshoot to reduce repeated sieving */
        if (next_limit < n * 5)
            next_limit = n * 5;
        if (next_limit == sieve_limit)
            next_limit = sieve_limit + 100;
        generate_up_to(next_limit);
    }
    return results[n - 1];
}
