/*
 * A324655  a(n) = A000005(A276086(n)).
 * https://oeis.org/A324655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of (digit+1) in the primorial‑base representation
 *            of n (digits < next_prime). This yields A000005( A276086(n) )
 * Range:     u8 n <= 358, u16 n <= 8318, u32 n <= 8318, u64 n <= 8318,
 *            u128 n <= 8318, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const idx_t _primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL,
    109ULL, 113ULL, 127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL,
    157ULL, 163ULL, 167ULL, 173ULL, 179ULL, 181ULL, 191ULL, 193ULL,
    197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL, 233ULL, 239ULL
};
static const size_t _PRIMES_CNT = sizeof(_primes)/sizeof(_primes[0]);

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 by definition */
        return 1;

    /* compute primorial weights up to n */
    idx_t weights[64];
    size_t k = 0;                     /* current prime index */
    weights[0] = 1;                   /* weight for position 0 */
    if (weights[0] > n) {
        /* cannot happen for n>0 because 1 <= n */
        return 1;
    }

    while (k + 1 < 64) {
        /* next weight = previous weight * next prime */
        if (weights[k] > n / _primes[k]) { /* would overflow or exceed n */
            break;
        }
        weights[k+1] = weights[k] * _primes[k];
        if (weights[k+1] > n) {
            break;
        }
        k++;
    }
    /* k now points to the largest index with weight <= n */
    size_t last = k;

    seq_t product = 1;
    idx_t rest = n;

    for (size_t i = last; ; --i) {
        /* digit = rest / weight[i] (digit < prime[i]) */
        idx_t digit = rest / weights[i];
        if (digit >= _primes[i]) {
            digit %= _primes[i];
        }
        product *= (seq_t)(digit + 1);
        rest -= digit * weights[i];
        if (i == 0) break;
    }

    return product;
}
