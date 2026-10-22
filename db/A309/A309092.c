/*
 * A309092  Integers whose hexadecimal representation contains a run of zeros
 *          of prime length.
 * https://oeis.org/A309092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth integer whose hexadecimal representation contains a *
 *            run of zeros of prime length
 * Range:     u8 none, u16 n <= 480, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* small helper: test primality of an unsigned integer up to a few hundred */
static bool is_prime_len(unsigned int m)
{
    if (m < 2) return false;
    for (unsigned int i = 2; i * i <= m; ++i)
        if (m % i == 0) return false;
    return true;
}

/* test whether x has a contiguous run of zeros of prime length in base‑16 */
static bool has_prime_zero_run(seq_t x)
{
    unsigned int run = 0;
    while (x != 0)
    {
        unsigned digit = (unsigned)(x & 15);
        if (digit == 0)
            ++run;
        else
        {
            if (run > 0 && is_prime_len(run))
                return true;
            run = 0;
        }
        x >>= 4;
    }
    return run > 0 && is_prime_len(run);
}

/* cache of previously found terms */
static seq_t *cached = NULL;       /* array of terms */
static size_t cached_cap = 0;      /* capacity */
static size_t cached_len = 0;      /* number of terms stored */
static seq_t next_value = 256;     /* next integer to examine */

/* required external linkage */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1 */

    /* generate terms until we have at least n of them */
    while (cached_len < (size_t)n)
    {
        /* find the next integer that satisfies the condition */
        while (!has_prime_zero_run(next_value))
            next_value = next_value + 1;

        /* store it in the cache, growing the array if necessary */
        if (cached_len >= cached_cap)
        {
            size_t new_cap = cached_cap == 0 ? 16 : cached_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure (unlikely) */
            cached = tmp;
            cached_cap = new_cap;
        }
        cached[cached_len++] = next_value;

        /* advance to the next integer for subsequent searches */
        next_value = next_value + 1;
    }

    return cached[n - 1];  /* sequence indices start at 1 */
}
