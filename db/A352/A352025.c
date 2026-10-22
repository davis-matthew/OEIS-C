/*
 * A352025  a(n) = floor(sqrt(6*prime(n))).
 * https://oeis.org/A352025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(6 * prime(n)))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t* prime_cache = NULL;   /* 1‑based storage of primes */
static size_t prime_count = 0;          /* number of primes stored */
static size_t prime_cap   = 0;          /* capacity of prime_cache */

/* --------------------------------------------------------------------- */
/*  Integer square root for 64‑bit values                                 */
/*  Returns ⌊√n⌋ for 0 ≤ n < 2⁶⁴.                                         */
/* --------------------------------------------------------------------- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = 1ULL << 32;           /* √(2⁶⁴−1) < 2³² */
    while (low < high)
    {
        uint64_t mid = (low + high) / 2;
        if (mid == 0)
        {
            low = 1;
            continue;
        }
        if (mid > n / mid)    /* mid*mid > n   */
            high = mid;
        else                   /* mid*mid ≤ n   */
        {
            if ((mid + 1) > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return low;
}

/* --------------------------------------------------------------------- */
/*  Ensure that prime_cache holds at least n primes (1‑based).            */
/* --------------------------------------------------------------------- */
static void ensure_primes(idx_t n)
{
    if (n <= prime_count)
        return;

    /* Grow cache if necessary */
    if (prime_cap < (size_t)n)
    {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp)
            abort();                       /* memory exhausted */
        prime_cache = tmp;
        prime_cap   = new_cap;
    }

    if (prime_count == 0)
    {   /* first prime */
        prime_cache[0] = 2;
        prime_count = 1;
    }

    uint64_t cand = prime_cache[prime_count - 1] + 1;
    if ((cand & 1) == 0)   /* make odd */
        cand++;

    while (prime_count < (size_t)n)
    {
        uint64_t sqrt_cand = isqrt_u64(cand);
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_cache[i] <= sqrt_cand; ++i)
        {
            if (cand % prime_cache[i] == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            prime_cache[prime_count++] = cand;
        }
        cand += 2;   /* next odd candidate */
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)      /* below the offset, undefined */
        return 0;

    ensure_primes(n);

    uint64_t p = prime_cache[n - 1];     /* 1‑based index */
    uint64_t val = 6ULL * p;             /* fits into 64‑bit for n ≤ 10⁴ */

    uint64_t sqrt_val = isqrt_u64(val);

    return (seq_t)sqrt_val;
}
