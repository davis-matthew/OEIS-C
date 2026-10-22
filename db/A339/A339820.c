/*
 * A339820  a(n) = phi(A019565(n)), where phi is Euler totient function.
 * https://oeis.org/A339820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of (prime_{i+1} - 1) over all set bits i in the
 *            binary representation of n. This equals φ of the product of the
 *            primes corresponding to those bits
 * Range:     u8 n <= 29, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const uint64_t primes[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL,41ULL,43ULL,
    47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,73ULL,79ULL,83ULL,89ULL,97ULL,101ULL,
    103ULL,107ULL,109ULL,113ULL,127ULL,131ULL,137ULL,139ULL,149ULL,151ULL,
    157ULL,163ULL,167ULL,173ULL,179ULL,181ULL,191ULL,193ULL,197ULL,199ULL,
    211ULL,223ULL,227ULL,229ULL,233ULL,239ULL,241ULL,251ULL,257ULL,263ULL,
    269ULL,271ULL,277ULL,281ULL,283ULL,293ULL,307ULL,311ULL
};
static const size_t prime_count = sizeof(primes)/sizeof(primes[0]);

seq_t a(idx_t n)
{
    idx_t bits = n;
    seq_t result = 1;
    for (size_t i = 0; bits && (i < prime_count); ++i)
    {
        if (bits & 1)
            result *= (seq_t)(primes[i] - 1);
        bits >>= 1;
    }
    return result;
}
