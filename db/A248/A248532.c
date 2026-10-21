/*
 * A248532  Numbers k such that the smallest prime divisor of k^2+1 is 53.
 * https://oeis.org/A248532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th term of OEIS A248532:
 *  Numbers k such that the smallest prime divisor of k^2+1 is 53.
 *  The sequence is generated in increasing order by testing all candidates
 *  congruent to 30 or 76 modulo 106 and accepting those for which no
 *  prime smaller than 53 divides k^2+1.
 */
#include <stdbool.h>

static const int SMALL_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
};
static const int NUM_SMALL = sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

static inline bool candidate_ok(seq_t k)
{
    /* 53 must divide k^2+1 */
    seq_t r = k % 53;
    if ( ((r * r) + 1) % 53 != 0 )
        return false;

    /* no smaller prime may divide k^2+1 */
    for (int i = 0; i < NUM_SMALL; ++i) {
        int p = SMALL_PRIMES[i];
        seq_t rp = k % p;
        if ( ((rp * rp) + 1) % p == 0 )
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */

    idx_t m = 0;              /* block counter (each block size 106) */
    idx_t count = 0;          /* how many valid terms have been found */
    seq_t result = 0;

    for (;;) {
        seq_t k1 = (seq_t)(106 * m + 30);
        if (candidate_ok(k1)) {
            ++count;
            if (count == n) { result = k1; break; }
        }

        seq_t k2 = (seq_t)(106 * m + 76);
        if (candidate_ok(k2)) {
            ++count;
            if (count == n) { result = k2; break; }
        }

        ++m;
    }
    return result;
}
