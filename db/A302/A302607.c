/*
 * A302607  a(n) is the greatest possible least prime in any partition of
 *          prime(n) into three primes; n >= 4.
 * https://oeis.org/A302607
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest possible least prime in a partition of prime(n)
 *            into three primes. Computation: sieve primes up to a bound, then
 *            for each n test decreasing candidates
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t   sieve_limit      = 0;        /* current upper bound of the sieve   */
static uint8_t *prime_flags     = NULL;      /* 1 if index is prime, 0 otherwise   */
static idx_t  *prime_list      = NULL;      /* array of primes up to sieve_limit   */
static size_t  prime_count     = 0;          /* number of primes stored            */

/* -------------------------------------------------------------------------- */
/* Build or enlarge a sieve up to 'limit' (inclusive) and store primes.       */
static void
ensure_sieve_up_to(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                     /* already large enough */

    /* Allocate new sieve array */
    size_t sig = (size_t)(limit + 1);
    uint8_t *new_flags = (uint8_t *)malloc(sig);
    if (!new_flags)
        abort();
    /* Mark all as potential primes */
    for (size_t i = 0; i < sig; ++i)
        new_flags[i] = 1;
    if (limit >= 0) new_flags[0] = 0;
    if (limit >= 1) new_flags[1] = 0;

    /* Eratosthenes sieve */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (new_flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_flags[j] = 0;
        }
    }

    /* Count primes and build list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_flags[i])
            ++cnt;

    idx_t *new_list = NULL;
    if (cnt > 0) {
        new_list = (idx_t *)malloc(cnt * sizeof(idx_t));
        if (!new_list)
            abort();
        size_t pos = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (new_flags[i])
                new_list[pos++] = i;
        }
    }

    /* Replace old structures */
    free(prime_flags);
    free(prime_list);
    prime_flags = new_flags;
    prime_list  = new_list;
    prime_count = cnt;
    sieve_limit = limit;
}

/* -------------------------------------------------------------------------- */
/* Return the nth prime (1‑based) using the sieve. The sieve is enlarged if    */
/* necessary using a simple upper bound n * 20 + 100 (more than enough up     */
/* to n = 10000).  */
static idx_t
nth_prime(idx_t n)
{
    /* Simple bound: for n ≥ 6, p_n < n (log n + log log n) < 20n for n ≤ 10000 */
    idx_t bound = n * 20 + 100;
    if (bound < 20) bound = 20;      /* minimal bound for tiny n */
    ensure_sieve_up_to(bound);

    /* If sieve still too small (unlikely), double until enough primes are present */
    while (prime_count < (size_t)n) {
        bound *= 2;
        ensure_sieve_up_to(bound);
    }

    return prime_list[n - 1];
}

/* -------------------------------------------------------------------------- */
/* Compute a(n) for n >= 4.  */
seq_t
a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;          /* undefined for n < 4 */

    /* Obtain prime(n) */
    idx_t p = nth_prime(n);       /* p is odd for n >= 4 */

    /* Ensure sieve covers numbers up to p for primality tests */
    if (sieve_limit < p)
        ensure_sieve_up_to(p);

    /* Find greatest possible least prime in partition p = a + b + c (a ≤ b ≤ c) */
    idx_t a_start = p / 3;        /* maximal a is floor(p/3) */
    for (idx_t a = a_start; a >= 2; --a) {
        if (!prime_flags[a])
            continue;            /* a must be prime */
        idx_t remain = p - a;     /* need b + c = remain, b ≤ c, b ≥ a */
        idx_t max_b = remain / 2; /* because b ≤ c ↔ b ≤ (remain/2)        */
        for (idx_t b = a; b <= max_b; ++b) {
            if (!prime_flags[b])
                continue;        /* b must be prime */
            idx_t r = remain - b; /* r = c */
            if (r < b)
                continue;        /* enforce b ≤ c */
            if (prime_flags[r])  /* found a valid partition */
                return (seq_t)a;
        }
    }

    /* Should not occur because of Goldbach's weak conjecture */
    return (seq_t)2;
}
