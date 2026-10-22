/*
 * A349669  a(n) is the n-th Sophie Germain prime reduced mod n.
 * https://oeis.org/A349669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005384(n) mod n
 * Range:     u8 n <= 303, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for prime and Sophie Germain prime lists */
static uint64_t *primes      = NULL;
static size_t   primes_count = 0, primes_capacity = 0;

static uint64_t *sg_primes   = NULL;
static size_t   sg_count     = 0, sg_capacity   = 0;

/* next number to test for primality */
static uint64_t next_candidate = 2;

/* multiply a vector by two */
static void grow_array(uint64_t **arr, size_t *cap, size_t *cnt)
{
    if (*cnt == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        if (tmp == NULL)
            abort();                     /* allocation failed */
        *arr = tmp;
        *cap = newcap;
    }
}

/* trial‑division primality test using the primes array */
static bool is_prime(uint64_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Ensure that at least 'need' Sophie Germain primes have been generated. */
static void ensure_sg_primes(size_t need)
{
    while (sg_count < need) {
        /* test current candidate for primality */
        if (is_prime(next_candidate)) {
            /* store the new prime */
            grow_array(&primes, &primes_capacity, &primes_count);
            primes[primes_count++] = next_candidate;

            /* test the Sophie Germain condition: 2p+1 prime */
            uint64_t q = (next_candidate << 1) + 1;
            if (is_prime(q)) {
                grow_array(&sg_primes, &sg_capacity, &sg_count);
                sg_primes[sg_count++] = next_candidate;
            }
        }

        /* advance to next candidate */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined but safe fallback */
        return (seq_t)0;

    /* generate enough Sophie Germain primes for n */
    ensure_sg_primes((size_t)n);

    uint64_t sg = sg_primes[(size_t)n - 1];
    seq_t res = (seq_t)(sg % (uint64_t)n);
    return res;
}
