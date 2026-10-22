/*
 * A361691  Number of divisors of 7*n-1 of form 7*k+1.
 * https://oeis.org/A361691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of divisors of (7*n-1) that are ≡ 1 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = |{d | d divides 7*n-1 and d ≡ 1 (mod 7)}| */

#include <stdint.h>
#include <string.h>

/* List of primes up to 263 (√69999 ≈ 264) */
static const uint64_t _primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL,
    83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL,
    109ULL, 113ULL, 127ULL, 131ULL, 137ULL, 139ULL,
    149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
    179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL,
    211ULL, 223ULL, 227ULL, 229ULL, 233ULL, 239ULL,
    241ULL, 251ULL, 257ULL, 263ULL
};
#define _NUM_PRIMES (sizeof(_primes)/sizeof(_primes[0]))

seq_t a(idx_t n)
{
    /* m = 7*n - 1 (fits in 64 bits for all n used in tests) */
    uint64_t m = 7ULL * (uint64_t)n - 1ULL;

    /* Prime factorisation of m */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pf_cnt = 0;
    uint64_t tmp = m;

    for (size_t pi = 0; pi < _NUM_PRIMES && _primes[pi] * _primes[pi] <= tmp; ++pi) {
        uint64_t p = _primes[pi];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pf_cnt]   = p;
            exps[pf_cnt]    = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        primes[pf_cnt]   = tmp;
        exps[pf_cnt]    = 1;
        ++pf_cnt;
    }

    /* Counting divisors by residue modulo 7 */
    unsigned long long cnt[7] = {0};
    cnt[1] = 1;  /* product of no primes = 1 */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* ways[res] = number of ways to get residue 'res' using this prime */
        unsigned long long ways[7] = {0};
        uint64_t residue = 1 % 7;   /* p^0 */
        ways[residue]++;            /* exponent 0 */
        for (uint64_t k = 1; k <= e; ++k) {
            residue = (residue * (p % 7)) % 7;
            ways[residue]++;
        }

        /* convolution of current counts with ways */
        unsigned long long newcnt[7] = {0};
        for (int a = 0; a < 7; ++a) {
            if (cnt[a] == 0) continue;
            for (int b = 0; b < 7; ++b) {
                if (ways[b] == 0) continue;
                int r = (a * b) % 7;
                newcnt[r] += cnt[a] * ways[b];
            }
        }
        memcpy(cnt, newcnt, sizeof(cnt));
    }

    /* Result: count of divisors ≡ 1 (mod 7) */
    return (seq_t)cnt[1];
}
