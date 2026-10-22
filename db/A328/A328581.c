/*
 * A328581  Product of nonzero digits in primorial base expansion of n.
 * https://oeis.org/A328581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of nonzero digits in primorial base expansion of
 *            n
 * Range:     u8 n <= 1467, u16 n <= 9149, u32 n <= 9149, u64 n <= 9149,
 *            u128 n <= 9149, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute primorial base digits of n and multiply all nonzero digits. */

#include <stdint.h>
#include <stddef.h>

/* first few primes are enough for n up to 2^64; we generate primorials until > n. */
static const uint64_t PRIMES[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,
    73ULL,79ULL,83ULL,89ULL,97ULL,101ULL,103ULL,107ULL,109ULL,113ULL
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* empty product */

    /* store all primorial weights <= n, always include weight 1 */
    uint64_t weights[64];
    size_t wcnt = 0;
    uint64_t weight = 1;             /* P_0 = 1 */
    weights[wcnt++] = weight;        /* digit 0 weight */

    for (size_t i = 0; i < sizeof(PRIMES)/sizeof(PRIMES[0]); ++i) {
        unsigned __int128 next = (unsigned __int128)weight * PRIMES[i];
        if (next > n) break;         /* further digits would be zero */
        weight = (uint64_t)next;
        weights[wcnt++] = weight;    /* store weight for next digit */
    }

    seq_t prod = 1;
    uint64_t m = n;

    /* process digits from most significant to least */
    for (int i = (int)wcnt - 1; i >= 0; --i) {
        uint64_t d = m / weights[i];
        if (d != 0) prod *= (seq_t)d;   /* multiply non‑zero digits */
        m -= d * weights[i];
    }
    return prod;
}
