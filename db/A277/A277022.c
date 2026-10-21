/*
 * A277022  Primorial base representation of n is rewritten as a base-2 number
 *          with each nonzero digit k replaced by a run of k 1's (followed by
 *          one extra zero if not the rightmost run of 1's) and with each 0
 *          kept as 0.
 * https://oeis.org/A277022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Convert n to its primorial‐base digits then replace each nonzero
 *            digit k by a run of k 1‑bits followed by a 0 if not the
 *            rightmost run. Zero digits become 0. The resulting binary number
 *            is a(n)
 * Range:     u8 n <= 28, u16 n <= 388, u32 n <= 8072, u64 n <= 8072,
 *            u128 n <= 8072, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* A small list of the first primes (enough for n up to 2^64). */
static const uint64_t _primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
    19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL,
    47ULL, 53ULL, 59ULL, 61ULL
};
static const size_t _primes_len = sizeof(_primes)/sizeof(_primes[0]);

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Compute primorials until the next one exceeds n. */
    /* prim[0] = 1; prim[i] = product of first i primes.           */
    size_t i = 0;
    seq_t cur = 1;
    /* 64 primorials are more than enough for 64‑bit indices.     */
    static seq_t prim[64];
    prim[0] = cur;

    while (cur <= n && i + 1 < 64 && i + 1 < _primes_len) {
        ++i;
        cur = cur * (seq_t)_primes[i-1];
        prim[i] = cur;
    }
    size_t k = i;   /* highest non–zero position */

    /* Compute the primorial‐base digits (most significant first). */
    static idx_t digits[64];
    idx_t rem = n;
    for (int j = (int)k; j >= 0; --j) {
        idx_t d = rem / (idx_t)prim[j];
        rem -= d * (idx_t)prim[j];
        digits[j] = d;
    }

    /* Find the lowest non‑zero digit (rightmost run). */
    int L = -1;
    for (size_t j = 0; j <= k; ++j) {
        if (digits[j] != 0) { L = (int)j; break; }
    }
    if (L == -1) return (seq_t)0;  /* should only happen for n==0 */

    /* Construct the binary number following the rule. */
    seq_t val = 0;
    for (int j = (int)k; j >= 0; --j) {
        idx_t d = digits[j];
        if (d == 0) {
            val <<= 1;                /* add a zero bit */
        } else {
            val <<= d;                /* shift to make space for the run */
            val |= (((seq_t)1 << d) - 1);  /* add d ones */
            if (j != L) val <<= 1;   /* add separating zero if not last run */
        }
    }
    return val;
}
