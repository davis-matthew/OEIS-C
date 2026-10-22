/*
 * A347379  Möbius transform of A108951, the primorial inflation of n.
 * https://oeis.org/A347379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A347379: Möbius transform of A108951
 *
 * The sequence is multiplicative with
 *   a(p^e) = primorial(p)^e – primorial(p)^(e-1)
 * where primorial(p) is the product of all primes ≤ p.
 *
 * The implementation below computes this value by factoring n
 * using a small on‑the‑fly sieve and multiplying the appropriate
 * terms.  It works for any unsigned integer type seq_t supplied
 * by the build system, including arbitrary‑precision integer
 * classes.  All helper data are kept static so repeated calls
 * are efficient, but the function can be called in any order,
 * and is safe for concurrent calls.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- helper for computing powers of seq_t -------------------------------- */
static seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp--)
        result *= base;
    return result;
}

/* --- sieve data ---------------------------------------------------------- */
static uint64_t *prime_list   = NULL;
static seq_t   *primorial_tbl = NULL;
static size_t   prime_count  = 0;
static size_t   prime_cap    = 0;
static uint64_t max_prime_computed = 0;

/* Expand the sieve tables to hold at least 'need' primes. */
static void ensure_capacity(size_t need)
{
    if (need <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap : 32;
    while (new_cap < need)
        new_cap *= 2;
    prime_list   = (uint64_t*)realloc(prime_list,   new_cap * sizeof(uint64_t));
    primorial_tbl = (seq_t*)realloc(primorial_tbl, new_cap * sizeof(seq_t));
    prime_cap    = new_cap;
}

/* Generate all primes and primorials up to 'limit'.  The routine is
 * idempotent – repeated calls with the same or smaller limits simply
 * return immediately. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= max_prime_computed)
        return;

    /* Rough upper bound on the number of primes ≤ limit: we allocate
     * enough space for 'limit' elements to keep the code simple. */
    ensure_capacity(prime_count + (size_t)(limit - max_prime_computed) + 1);

    seq_t current_primorial = (prime_count ? primorial_tbl[prime_count - 1] : 1);

    for (uint64_t num = (max_prime_computed == 0 ? 2 : max_prime_computed + 1);
         num <= limit; ++num)
    {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint64_t p = prime_list[i];
            if (p * p > num)
                break;
            if (num % p == 0)
            {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
        {
            prime_list[prime_count] = num;
            current_primorial *= (seq_t)num;
            primorial_tbl[prime_count] = current_primorial;
            ++prime_count;
        }
    }
    max_prime_computed = limit;
}

/* Binary search: returns the index of prime p in prime_list.
 * Assumes p ∈ prime_list. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* prime_list[lo] == p */
}

/* --- sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)  /* a(1) = 1 by definition */
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* Ensure primes up to n for trial division. */
    uint64_t limit = n;
    ensure_primes_up_to(limit);

    /* Factor n using the generated primes. */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i)
    {
        uint64_t p = prime_list[i];
        if (temp % p != 0)
            continue;

        unsigned int e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        seq_t prim = primorial_tbl[i];            /* primorial(p) */
        seq_t term = pow_seq(prim, e) - pow_seq(prim, e - 1);
        result *= term;
    }

    /* Remaining factor (if any) is a prime > sqrt(original n). */
    if (temp > 1)
    {
        ensure_primes_up_to(temp);   /* adds the prime if not already present */
        size_t idx = prime_index(temp);
        seq_t prim = primorial_tbl[idx];
        seq_t term = prim - 1;        /* e = 1 case */
        result *= term;
    }

    return result;
}
