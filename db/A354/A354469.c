/*
 * A354469  Write n in primorial base, then replace each nonzero digit d of
 *          radix p with p-d.
 * https://oeis.org/A354469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = transform(n) in primorial base Compute the primorial‑base
 *            digits of n, replace every non‑zero digit d of radix p by p - d,
 *            and interpret the resulting digits in the same mixed radix
 *            system
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* First several primes – enough for all n that fit
       in a 64‑bit unsigned integer (the product of the
       first 20 primes already exceeds 2^64). */
    static const uint64_t primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL,
        47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
        73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL,
        103ULL, 107ULL, 109ULL, 113ULL, 127ULL, 131ULL,
        137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL,
        167ULL, 173ULL, 179ULL, 181ULL, 191ULL, 193ULL,
        197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL
    };
    const size_t num_primes = sizeof(primes) / sizeof(primes[0]);

    idx_t m = n;            /* working copy of n */
    seq_t result = 0;       /* final value */
    seq_t mult = 1;         /* product of all previous primes (weight) */
    size_t pos = 0;         /* current radices index */

    while (1) {
        uint64_t p   = primes[pos];
        uint64_t d   = m % p;          /* digit in this radix */

        if (d != 0) {
            d = p - d;                 /* replace non‑zero digit */
            result += (seq_t)d * mult; /* add weighted contribution */
        }

        /* Update weight for next higher radix */
        mult *= (seq_t)p;

        m /= p;                         /* move to higher radix */
        if (m == 0) break;              /* finished all digits */

        pos++;
        /* The provided primes cover all practical n;
           this guard is only defensive. */
        if (pos >= num_primes) break;
    }

    return result;
}
