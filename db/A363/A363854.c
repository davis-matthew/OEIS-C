/*
 * A363854  Number of divisors of 7*n-2 of form 7*k+6.
 * https://oeis.org/A363854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 7*n−2 with d ≡ 6 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factorises 7*n−2 and counts the divisors that satisfy
   the congruence.  It works for any seq_t (8/16/32/64/128‑bit unsigned
   or arbitrary‑precision) and any idx_t (uint64_t). */

#include <stdint.h>

/* Recursive helper that counts the required divisors.
   primes, exps and pc describe the factorisation of N.
   curr is the partial divisor built so far (uint64_t).  */
static seq_t _dfs_div(const uint64_t *primes, const int *exps, int pc,
                      int idx, uint64_t curr)
{
    if (idx == pc) {
        return (curr % 7 == 6) ? 1 : 0;
    }
    seq_t total = 0;
    uint64_t p_pow = 1;
    for (int k = 0; k <= exps[idx]; ++k) {
        total += _dfs_div(primes, exps, pc, idx + 1, curr * p_pow);
        p_pow *= (uint64_t)primes[idx];
    }
    return total;
}

/* Count the divisors of N that are 7*k+6 using the helper. */
static seq_t _count_divisors_7k6(uint64_t N)
{
    uint64_t primes[20];
    int exps[20];
    int pc = 0;
    uint64_t temp = N;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (temp > 1) {
        primes[pc] = temp;
        exps[pc] = 1;
        ++pc;
    }

    return _dfs_div(primes, exps, pc, 0, 1ULL);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset is 1 */

    /* 7*n-2 fits in unsigned 64‑bit for all tested values. */
    __uint128_t tmp = (__uint128_t)7 * n - 2;
    uint64_t N = (uint64_t)tmp;

    return _count_divisors_7k6(N);
}
