/*
 * A354812  Positions of +2's in A346242.
 * https://oeis.org/A354812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 * p_{n+4} where p_k is the k-th prime (p_1 = 2). The
 *            sequence gives the positions of +2's in A346242, which start at
 *            12×11, 12×13, 12×17, …
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *primes = NULL;
static size_t primes_sz = 0;
static size_t primes_cap = 0;

/* Ensure that the first 'needed' primes are generated.
   'needed' is a 1‑based count (i.e., needed=5 means we need primes[4] == 11). */
static void ensure_primes(size_t needed)
{
    if (primes_sz >= needed)
        return;

    if (primes_cap < needed)
    {
        size_t new_cap = primes_cap ? primes_cap * 2 : 128;
        if (new_cap < needed)
            new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp)
            abort(); /* allocation failure */
        primes = tmp;
        primes_cap = new_cap;
    }

    /* start building from the last known prime */
    uint64_t cand;
    if (primes_sz == 0)
    {
        /* initial prime list */
        primes[0] = 2;
        primes_sz = 1;
        cand = 3;
    }
    else
    {
        cand = primes[primes_sz - 1] + 1;
        if (cand % 2 == 0)
            cand++;          /* ensure odd */
    }

    while (primes_sz < needed)
    {
        bool is_prime = true;
        for (size_t i = 0; i < primes_sz; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
            primes[primes_sz++] = cand;

        cand += 2; /* only check odd candidates */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1; n=0 is undefined but return 0 */

    size_t target = (size_t)n + 4;                 /* index of the prime */
    ensure_primes(target);
    /* target is 1‑based; primes vector is 0‑based */
    return (seq_t)12 * (seq_t)primes[target - 1];
}
