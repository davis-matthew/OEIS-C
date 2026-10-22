/*
 * A364034  Expansion of Sum_{k>0} x^k / (1 - 2*x^(2*k)).
 * https://oeis.org/A364034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364034: a(n) = Sum_{d|n, d odd} 2^((d-1)/2) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* Recursively enumerate all odd divisors of the odd part of n.
   primes[] and exps[] store the odd prime factors and their exponents.
   pc          – number of such factors.
   pos         – current depth in the recursion.
   cur         – divisor built so far.
   sum_out     – accumulator for a(n). */
static void dfs_divisors(const uint64_t primes[], const uint8_t exps[],
                         size_t pc, size_t pos, uint64_t cur,
                         seq_t *sum_out)
{
    if (pos == pc) {
        seq_t exp = ((seq_t)cur - 1) >> 1;          /* (cur-1)/2 */
        seq_t term = big_pow((seq_t)2, exp);        /* 2^exp */
        *sum_out += term;
        return;
    }

    uint64_t p = primes[pos];
    uint8_t e_max = exps[pos];
    uint64_t product = 1;                           /* p^e */

    for (uint8_t e = 0; e <= e_max; ++e) {
        uint64_t next_div = cur * product;
        dfs_divisors(primes, exps, pc, pos + 1, next_div, sum_out);

        /* Prepare product for next exponent (p^(e+1)) */
        __uint128_t tmp = (__uint128_t)product * p;
        product = (uint64_t)tmp;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* defined only for n >= 1 */

    /* Isolate the odd part of n. */
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0) odd >>= 1;

    /* Factor the odd part. */
    uint64_t primes[64];
    uint8_t exps[64];
    size_t pc = 0;
    uint64_t m = odd;

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint8_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (m > 1) {                         /* remaining prime factor */
        primes[pc] = m;
        exps[pc] = 1;
        ++pc;
    }

    seq_t sum = 0;
    dfs_divisors(primes, exps, pc, 0, 1ULL, &sum);
    return sum;
}
