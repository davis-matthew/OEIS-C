/*
 * A378423  a(n) is the number of distinct terms reached by iterating the
 *          function f(x) = 2 + A008472(x), starting from x=n.
 * https://oeis.org/A378423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378423: a(n) = number of distinct terms produced by iterating  
   f(x) = 2 + A008472(x) starting from x = n.  The orbit always ends in the
   fixed point 4 or in the cycle {5,7,9}.  Here we use Floyd's cycle‑finding
   algorithm to determine the tail length μ and the cycle length λ, and
   return μ + λ.  */

#include <stdint.h>
#include <stddef.h>

/* Return the sum of distinct prime factors of a 64‑bit integer. */
static uint64_t sum_distinct_prime_factors(uint64_t n)
{
    uint64_t sum = 0;
    if (n <= 1) return 0;
    if (n % 2 == 0) {
        sum += 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            sum += p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) sum += n;
    return sum;
}

/* f(x) = 2 + sum of distinct prime factors of x, cast to seq_t. */
static seq_t f(seq_t x)
{
    uint64_t val = (uint64_t)x;
    uint64_t s   = sum_distinct_prime_factors(val);
    return (seq_t)2 + (seq_t)s;
}

seq_t a(idx_t n)
{
    /* Step 1: find first meeting point of tortoise and hare. */
    seq_t tortoise = f((seq_t)n);
    seq_t hare = f(f((seq_t)n));
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    /* Step 2: find the start of the cycle (mu). */
    seq_t mu_ptr = (seq_t)n;
    size_t mu = 0;
    while (mu_ptr != tortoise) {
        mu_ptr = f(mu_ptr);
        tortoise = f(tortoise);
        mu++;
    }

    /* Step 3: determine the cycle length (lambda). */
    size_t lam = 1;
    hare = f(mu_ptr);
    while (hare != mu_ptr) {
        hare = f(hare);
        lam++;
    }

    /* Number of distinct terms equals mu + lambda. */
    return (seq_t)(mu + lam);
}
