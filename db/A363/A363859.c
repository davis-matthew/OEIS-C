/*
 * A363859  Number of divisors of 7*n-1 of the form 7*k+2.
 * https://oeis.org/A363859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 7*n‑1 such that d ≡ 2 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply the current residue array by the contributions of a prime
   p^e.  The array 'res' represents the number of ways to obtain each
   residue mod 7 from the already processed prime factors.  After
   calling, res is updated to include this prime.  Only a few states
   (0…6) are needed. */
static void combine_prime(uint64_t p, unsigned e, seq_t *res)
{
    int pmod = (int)(p % 7ULL);
    seq_t new_res[7] = {0};
    int cur = 1;                     /* p^k mod 7, k starts at 0 */
    for (unsigned k = 0; k <= e; ++k) {
        for (int r = 0; r < 7; ++r) {
            if (res[r] == 0) continue;
            int new_r = (r * cur) % 7;
            new_res[new_r] += res[r];
        }
        cur = (cur * pmod) % 7;
    }
    /* copy back */
    for (int i = 0; i < 7; ++i) res[i] = new_res[i];
}

/* Count divisors of a 64‑bit number m that are ≡ 2 (mod 7). */
static seq_t count_mod7_two(uint64_t m)
{
    seq_t res[7] = {0};
    res[1] = 1;                      /* product of zero primes is 1 */

    uint64_t tmp = m;

    /* factor 2 */
    unsigned e = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e > 0) combine_prime(2ULL, e, res);

    /* odd factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p) continue;
        e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        combine_prime(p, e, res);
    }
    if (tmp > 1)                     /* remaining prime factor */
        combine_prime(tmp, 1, res);

    return res[2];
}

seq_t a(idx_t n)
{
    /* 7*n-1 is at most 64‑bit for all reasonable n used in tests.
       We compute it in 64 bit for factorization. */
    uint64_t M = 7 * n - 1;
    return count_mod7_two(M);
}
