/*
 * A373606  Sum of the even-indexed digits in the primorial base
 *          representation (A049345) of n.
 * https://oeis.org/A373606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of even‑indexed digits in the primorial base
 *            representation of n
 * Range:     u8 n <= 9889, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* First 20 primes; primorials up to 2⁶⁴‑1 fit within the first 16 of them. */
static const uint64_t _primes[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};
static const size_t _PRIMES_COUNT = 20;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t tmp = n;                         /* working copy of n */
    uint64_t primorials[_PRIMES_COUNT + 1];
    primorials[0] = 1;                     /* P₀ = 1 for the least significant digit */
    size_t count = 1;                      /* number of primorials stored so far */

    /* Build primorials up to the largest ≤ n. */
    uint64_t prim = 1;
    for (size_t i = 0; i < _PRIMES_COUNT; ++i)
    {
        unsigned __int128 product = (unsigned __int128)prim * _primes[i];
        if (product > (unsigned __int128)n) break;
        prim = (uint64_t)product;
        primorials[count++] = prim;
    }

    seq_t sum = 0;

    /* Extract digits from most significant to least significant position. */
    for (int i = (int)count - 1; i >= 0; --i)
    {
        uint64_t base = primorials[i];
        idx_t digit = tmp / base;
        tmp -= (idx_t)digit * base;
        if ((i & 1) == 0)          /* even position: sum digit */
            sum += (seq_t)digit;
    }

    return sum;
}
