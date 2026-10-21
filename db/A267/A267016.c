/*
 * A267016  Numbers that satisfy: (isqrt(n)-1)! = isqrt(n)-1 mod isqrt(n).
 * https://oeis.org/A267016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 9601, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267016: Numbers n such that floor(sqrt(n)) is 1 or a prime.
   The sequence consists of all integers in the blocks
        [p² , (p+1)² – 1] for p = 1 or any prime p.
   a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, …                    */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ----- integer square root for uint64_t ---------------------------------- */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t x = n, y;
    if (x == 0) return 0;
    while (1)
    {
        y = (x + n / x) >> 1;
        if (y >= x) return x;
        x = y;
    }
}

/* ----- prime generation ------------------------------------------------- */
static uint64_t *prime_list   = NULL; /* primes in ascending order */
static size_t   prime_cnt     = 0;
static size_t   prime_cap     = 0;

/* Add the next prime after the current list. */
static void add_next_prime(void)
{
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    while (1)
    {
        /* check divisibility by known primes up to sqrt(candidate) */
        bool is_p = true;
        uint64_t lim = int_sqrt_u64(candidate);
        for (size_t j = 0; j < prime_cnt && prime_list[j] <= lim; ++j)
        {
            if (candidate % prime_list[j] == 0)
            {
                is_p = false;
                break;
            }
        }

        if (is_p)
        {
            /* resize array if needed */
            if (prime_cnt >= prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = candidate;
            return;
        }
        ++candidate;
    }
}

/* ----- a(n) -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initial segment for floor sqrt = 1 : numbers 1, 2, 3 */
    if (n <= 3) return (seq_t)n;

    idx_t counted = 3; /* terms already accounted for */
    size_t idx = 0;    /* index into prime_list */

    while (1)
    {
        /* Ensure we have a prime at position idx */
        while (idx >= prime_cnt) add_next_prime();

        uint64_t p = prime_list[idx++];
        /* compute block start = p^2 (in seq_t) */
        seq_t pp = (seq_t)p;
        seq_t block_start = pp * pp;

        /* block length = 2p + 1 (fits in idx_t) */
        idx_t block_len = (idx_t)(2u * (uint32_t)p + 1u);

        if (counted + block_len >= n)
        {
            idx_t offset = n - counted - 1;
            return block_start + (seq_t)offset;
        }

        counted += block_len;
    }
}
