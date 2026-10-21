/*
 * A275866  Number of semiprimes in {n, f(n), f(f(n)), ...., 1}, where f is
 *          the Collatz function.
 * https://oeis.org/A275866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of semiprimes in the Collatz trajectory of n *
 *            (trajectory: n, f(n), f(f(n)), …, 1; f(x) = 3x+1 if x odd, x/2
 *            if even). * This implementation uses trial division to test
 *            semiprimality * and works for any seq_t and idx_t type supplied
 *            by the build
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether a 64‑bit integer is a semiprime (exactly two prime
 * factors, counted with multiplicity). */
static inline bool is_semiprime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    int count = 0;

    /* factor 2 repeatedly */
    while ((n & 1ULL) == 0) {
        count++;
        if (count > 2)
            return false;
        n >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        while (n % p == 0) {
            count++;
            if (count > 2)
                return false;
            n /= p;
        }
    }

    /* if any prime factor remains > 1 */
    if (n > 1)
        count++;

    return (count == 2);
}

seq_t a(idx_t n)
{
    seq_t semiprime_count = (seq_t)0;
    uint64_t cur = (uint64_t)n;

    while (1) {
        if (is_semiprime_uint64(cur))
            semiprime_count += (seq_t)1;

        if (cur == 1)
            break;

        if (cur & 1ULL)           /* odd */
            cur = 3ULL * cur + 1ULL;
        else                      /* even */
            cur >>= 1ULL;
    }

    return semiprime_count;
}
