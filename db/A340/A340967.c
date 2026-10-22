/*
 * A340967  a(n) is the number of iterations of the map x -> n mod sopfr(x)
 *          starting with n to reach 0 or 1, where sopfr = A001414.
 * https://oeis.org/A340967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1081, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of iterations of
 *             x -> n mod sopfr(x) starting with x = n
 *             until the value reaches 0 or 1.
 */
#include <stdint.h>
#include <stddef.h>

/* sum of prime factors with multiplicity (sopfr) for a 64‑bit unsigned integer */
static seq_t sopfr_uint64(uint64_t x)
{
    seq_t sum = 0;
    if (x <= 1) return sum;

    /* factor out the twos */
    while ((x & 1ULL) == 0) {
        sum += (seq_t)2;
        x >>= 1;
    }

    /* factor out odd primes */
    for (uint64_t i = 3; i <= x / i; i += 2) {
        while (x % i == 0) {
            sum += (seq_t)i;
            x /= i;
        }
    }

    /* any remaining prime factor > sqrt(original) */
    if (x > 1) {
        sum += (seq_t)x;
    }

    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    uint64_t n64 = (uint64_t)n;          /* keep original n for mod operations */
    uint64_t x = n64;                    /* current value in the iteration */
    seq_t  cnt = 0;                      /* number of iterations */

    while (x > 1) {
        seq_t p = sopfr_uint64(x);               /* sopfr(x) */
        seq_t r = (seq_t)n64 % p;                /* n mod sopfr(x) */
        x = (uint64_t)r;                         /* next value */
        ++cnt;
    }

    return cnt;
}
