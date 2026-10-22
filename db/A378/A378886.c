/*
 * A378886  The number of consecutive primes in the prime factorization of n
 *          starting from the smallest prime dividing n; a(1) = 0.
 * https://oeis.org/A378886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378886:  
 * a(n) = number of consecutive primes in the prime factorization of n
 * starting from the smallest prime dividing n.  a(1)=0.  */
#include <stdbool.h>
#include <stdint.h>

static bool is_prime_local(idx_t x)
{
    if (x <= 1) return false;
    if (x <= 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (idx_t i = 5; i <= x / i; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0; the sequence is undefined for n < 1. */
    if (n <= 1) return 0;

    /* Find the smallest prime factor of n. */
    idx_t smallest = 0;
    if (n % 2 == 0) {
        smallest = 2;
    } else {
        for (idx_t p = 3; p <= n / p; p += 2) {
            if (n % p == 0) {
                smallest = p;
                break;
            }
        }
        if (smallest == 0) smallest = n; /* n is prime */
    }

    seq_t count = 1;          /* the smallest prime factor itself */
    idx_t current = smallest;

    /* Scan through successive primes, stopping when a prime
       that divides n is not found. */
    idx_t candidate = current + 1;
    while (candidate <= n) {
        if (!is_prime_local(candidate)) {
            ++candidate;
            continue;
        }
        if (n % candidate != 0) break;   /* chain broken */
        ++count;
        current = candidate;
        ++candidate;
    }

    return count;
}
