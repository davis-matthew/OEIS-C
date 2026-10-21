/*
 * A277692  Mendelsohn-Rodney sequence: number of court balanced tournament
 *          designs that are available for a given set of teams n.
 * https://oeis.org/A277692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors c of gcd(C(n,2), n-1) such that 1 ≤ c
 *            ≤ ⌊n/2⌋
 * Range:     u8 n <= 24, u16 n <= 1748, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Count divisors of the integer represented by the prime factorization
   given by arrays `primes` and `exps` (length `parts`) that are ≤ `limit`. */
static void count_divisors(uint64_t idx, uint64_t curr, uint64_t limit,
                           const uint64_t *primes, const int *exps,
                           int parts, int *cnt)
{
    if (idx == parts) {
        /* All primes processed; current divisor is `curr`. */
        if (curr <= limit)
            (*cnt)++;
        return;
    }

    const uint64_t p = primes[idx];
    const int    emax = exps[idx];
    uint64_t val = 1;

    for (int e = 0; e <= emax; ++e) {
        uint64_t newcurr = curr * val;
        if (newcurr > limit) break;
        count_divisors(idx + 1, newcurr, limit, primes, exps, parts, cnt);

        if (e < emax) {
            /* Prepare val *= p for next exponent, checking overflow. */
            if (val > limit / p) break;
            val *= p;
        }
    }
}

/* OEIS sequence A277692 */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0 */
    if (n == 1)
        return 0;

    /* C(n,2) using seq_t arithmetic to avoid overflow. */
    seq_t comb = (seq_t)n * (seq_t)(n - 1) / 2;

    /* Common divisor of C(n,2) and n-1. */
    seq_t g = gcd(comb, (seq_t)(n - 1));
    if (g == 0)
        return 0;   /* Only possible for n = 1, already handled */

    /* Convert gcd to uint64_t for factorisation (safe for n ≤ 10 000). */
    uint64_t g64 = (uint64_t)g;

    /* Factor g by trial division. */
    uint64_t primes_arr[20];
    int      exps_arr[20];
    int      parts = 0;
    uint64_t tmp = g64;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            primes_arr[parts] = p;
            exps_arr[parts] = cnt;
            ++parts;
        }
    }
    if (tmp > 1) {
        primes_arr[parts] = tmp;
        exps_arr[parts] = 1;
        ++parts;
    }

    /* Count divisors ≤ floor(n/2). */
    uint64_t limit = n / 2;
    int divcnt = 0;
    count_divisors(0, 1, limit, primes_arr, exps_arr, parts, &divcnt);

    return (seq_t)divcnt;
}
