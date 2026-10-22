/*
 * A329900  Primorial deflation of n: starting from x = n, repeatedly divide x
 *          by the largest primorial A002110(k) that divides it, until x is an
 *          odd number. Then a(n) = Product prime(k_i), for primorial indices
 *          k_1 >= k_2 >= ..., encountered in the process.
 * https://oeis.org/A329900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the primes of the successive largest
 *            primorials that divide n until the remaining factor is odd. If n
 *            is odd the result is 1
 * Range:     u8 n <= 255, u16 n <= 9946, u32 n <= 9946, u64 n <= 9946,
 *            u128 n <= 9946, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps a small table of the first primes.  The largest
    primorial that can divide a 64‑bit n is the primorial of the 15th prime
    (47).  For 128‑bit integers a few more primes are needed, but 100
    primes are more than enough for any practical n that appears in the
    tests.  All operations use only + – * / % and bitwise operators,
    as required. */

#include <stdint.h>
#include <stddef.h>          /* for size_t */

/* First 100 primes (up to 541).  Enough for every n in the required range. */
static const uint32_t primes[] = {
    2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
   31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
   73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
  127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
  179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
  233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
  283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
  353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
  419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
  467, 479, 487, 491, 499, 503, 509, 521, 523, 541
};
static const size_t prime_count = sizeof(primes)/sizeof(primes[0]);

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* work in seq_t to handle large numbers */
    seq_t result = 1;

    /* Repeat until the remaining factor is odd.  For odd n the loop is skipped. */
    while ((x & 1) == 0) {
        seq_t primorial = 1;     /* product of the first k primes that divide x */
        size_t i;

        for (i = 0; i < prime_count; ++i) {
            if (x % (seq_t)primes[i] == 0) {
                primorial *= (seq_t)primes[i];
            } else {
                break;
            }
        }

        /* i is at least 1 because x is even (so 2 divides it) */
        result *= (seq_t)primes[i-1];
        x /= primorial;
    }

    return result;
}
