/*
 * A256468  Number of primes between prime(n)^2 and prime(n)*prime(n+1).
 * https://oeis.org/A256468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 3076, u32 n <= 6542, u64 n <= 6542,
 *            u128 n <= 6542, big any n
 * Helpers:   _floor_isqrt_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A256468: Number of primes between prime(n)^2 and prime(n)*prime(n+1).
   The code uses a simple prime sieve and a segmented sieving routine to
   count the primes in the required interval.  It works for any build
   where seq_t is an unsigned integer type (or an arbitrary‑precision
   integer class) and idx_t is unsigned 64‑bit.  All allocations are
   cast to the correct pointer type so that the code compiles as
   C++17 as well as C11.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/_floor_isqrt_u64.h"

static uint32_t *prime_list   = NULL;      /* primes up to primes_precomputed_to          */
static size_t   prime_cnt     = 0;          /* number of primes in prime_list             */
static idx_t    primes_precomputed_to = 0;  /* maximum prime precomputed so far (inclusive) */

/* Ensure prime_list contains all primes <= limit. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_precomputed_to)
        return;

    idx_t sieve_limit = limit + 1;
    bool *is_composite = (bool *)calloc((size_t)sieve_limit, sizeof(bool));
    if (!is_composite)
        exit(1); /* memory exhaustion */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;

    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++new_cnt;

    uint32_t *new_list = (uint32_t *)realloc(prime_list,
                                             new_cnt * sizeof(uint32_t));
    if (!new_list)
        exit(1); /* memory exhaustion */
    prime_list = new_list;
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            prime_list[prime_cnt++] = (uint32_t)i;

    primes_precomputed_to = limit;
    free(is_composite);
}

/* Count primes in the open interval (low, high). */
static seq_t count_primes_in_range(idx_t low, idx_t high)
{
    if (high <= low + 1)   /* no integers in the interval              */
        return (seq_t)0;

    idx_t segment_size_idx = high - low - 1;          /* size of (low, high)      */
    if (segment_size_idx > SIZE_MAX)
        exit(1);  /* size_t overflow                         */
    size_t segment_size = (size_t)segment_size_idx;

    unsigned char *segment = (unsigned char *)calloc(segment_size, sizeof(unsigned char));
    if (!segment)
        exit(1); /* memory exhaustion */

    idx_t sqrt_high = _floor_isqrt_u64(high - 1);

    for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= sqrt_high; ++pi)
    {
        idx_t p = (idx_t)prime_list[pi];
        idx_t start = (low / p + 1) * p;
        for (idx_t j = start; j < high; j += p)
            segment[(size_t)(j - low - 1)] = 1;
    }

    seq_t cnt = 0;
    for (size_t i = 0; i < segment_size; ++i)
        if (!segment[i])
            ++cnt;

    free(segment);
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* below offset, undefined but return 0 */

    idx_t p_n   = get_prime(n);
    idx_t p_np1 = get_prime(n + 1);

    /* Ensure we have all primes up to p_np1 for sieving */
    if (p_np1 > primes_precomputed_to)
        ensure_primes_up_to(p_np1);

    idx_t low  = p_n * p_n;          /* prime(n)^2          */
    idx_t high = p_n * p_np1;        /* prime(n)*prime(n+1) */

    return count_primes_in_range(low, high);
}
