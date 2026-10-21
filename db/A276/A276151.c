/*
 * A276151  n minus the greatest primorial number (A002110) which divides n:
 *          a(n) = n - A053589(n).
 * https://oeis.org/A276151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n minus the greatest primorial (A002110) that divides n
 * Range:     u8 n <= 255, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t primes[] = {
    2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul,
    31ul, 37ul, 41ul, 43ul, 47ul, 53ul, 59ul, 61ul, 67ul,
    71ul, 73ul, 79ul, 83ul, 89ul, 97ul          /* 25 primes – enough for all practical n */
};
static const size_t prime_cnt = sizeof(primes) / sizeof(primes[0]);

seq_t a(idx_t n)
{
    /* n >= 1 for the sequence offset.  Behaviour for n == 0 is undefined
       or simply return 0; here we guard for safety. */
    if (n == 0u)
        return (seq_t)0;

    seq_t n_t = (seq_t)n;
    seq_t best = (seq_t)1;          /* 1 always divides n */
    seq_t primorial = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t prime = (seq_t)primes[i];
        /* Stop if next multiplication would exceed n_t (or overflow). */
        if (primorial > n_t / prime)
            break;
        primorial = primorial * prime;
        if (n_t % primorial == 0)
            best = primorial;
    }

    return n_t - best;
}
