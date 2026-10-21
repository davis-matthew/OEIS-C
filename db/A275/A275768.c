/*
 * A275768  a(n) is the number of ways to express n = (prime(i) + prime(j))/2
 *          when (prime(i) - prime(j))/2 also is prime.
 * https://oeis.org/A275768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of primes C with C ≤ n-2 such that * n-C and
 *            n+C are prime (C is also prime). This counts * the unordered
 *            pairs of primes (p,q) with p+q=2n and * (p-q)/2 prime
 * Range:     u8 n <= 6929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <math.h>

static bool *prime_flag = NULL;          /* sieve array */
static size_t sieve_limit = 0;          /* current maximal index sieved */

/* Resize the prime sieve so that indices 0..limit are valid.  The sieve
 * is rebuilt from scratch each time it grows.  Only grows when necessary. */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit)
        return;

    bool *new_flag = (bool*)malloc((limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= limit; ++i)
        new_flag[i] = true;
    if (limit >= 0) new_flag[0] = false;
    if (limit >= 1) new_flag[1] = false;

    size_t sqrt_lim = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= sqrt_lim; ++p)
        if (new_flag[p]) {
            for (size_t mult = p * p; mult <= limit; mult += p)
                new_flag[mult] = false;
        }

    free(prime_flag);
    prime_flag = new_flag;
    sieve_limit = limit;
}

/* Compute a(n) as defined above.  The function is self‑contained and
 * keeps a static sieve that grows as necessary. */
seq_t a(idx_t n)
{
    if (n < 2)          /* for n=0,1 the count is zero */
        return 0;

    /* We need primes up to 2n (for n+C) and up to n-2 (for C). */
    size_t needed_limit = (size_t)(2 * (unsigned long long)n);
    ensure_sieve(needed_limit);

    seq_t count = 0;
    idx_t max_c = n - 2;                     /* largest valid C */
    for (idx_t c = 2; c <= max_c; ++c) {
        if (!prime_flag[c])
            continue;
        if (!prime_flag[(size_t)(n - c)])
            continue;
        if (!prime_flag[(size_t)(n + c)])
            continue;
        ++count;
    }
    return count;
}
