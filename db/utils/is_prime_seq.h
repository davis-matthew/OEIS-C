/* is_prime_seq: whether n is prime.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_SEQ_H
#define OEISC_UTILS_IS_PRIME_SEQ_H
#include <stdbool.h>
#include <stdint.h>

/* Whether n is prime. A value that fits in 64 bits -- every value at a fixed
   width up to 64, and nearly every one of a wider seq_t -- goes through a
   deterministic Miller-Rabin test in uint64_t, which is also what keeps the
   arbitrary-precision build fast; a larger one is tested by trial division
   in seq_t. */
static uint64_t is_prime_seq_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static bool is_prime_seq_u64(uint64_t n)
{
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2)
        return false;
    for (int i = 0; i < 12; i++)
        if (n % bases[i] == 0)
            return n == bases[i];
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    for (int i = 0; i < 12; i++) {
        uint64_t x = 1, b = bases[i], e = d;
        while (e) {
            if (e & 1)
                x = is_prime_seq_mulmod(x, b, n);
            b = is_prime_seq_mulmod(b, b, n);
            e >>= 1;
        }
        if (x == 1 || x == n - 1)
            continue;
        int r;
        for (r = 1; r < s; r++) {
            x = is_prime_seq_mulmod(x, x, n);
            if (x == n - 1)
                break;
        }
        if (r == s)
            return false;
    }
    return true;
}

static bool is_prime_seq(seq_t n)
{
    if (n < (seq_t)2)
        return false;
    uint64_t low = (uint64_t)n;
    if ((seq_t)low == n)
        return is_prime_seq_u64(low);
    if (n % (seq_t)2 == (seq_t)0)
        return false;
    for (seq_t i = (seq_t)3; i <= n / i; i += (seq_t)2)
        if (n % i == (seq_t)0)
            return false;
    return true;
}

#endif
