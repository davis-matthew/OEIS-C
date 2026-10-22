/*
 * A327695  Number of non-constant factorizations of n whose distinct factors
 *          are pairwise coprime.
 * https://oeis.org/A327695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into more than
 *            one factor whose distinct factors are pairwise coprime
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* small gcd for uint64_t */
static uint64_t _gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* number of divisors of x (x>0) */
static seq_t _divisor_count(uint64_t x) {
    seq_t cnt = 1;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t c = 0;
            while (x % p == 0) {
                x /= p;
                c++;
            }
            cnt *= (c + 1);
        }
    }
    if (x > 1) cnt *= 2;
    return cnt;
}

/* recursive enumeration of partitions of the prime indices [0..cnt-1]  */
static void _dfs(int idx, int groups,
                 const uint64_t e[], int cnt,
                 uint64_t curg[], seq_t *total) {
    if (idx == cnt) {
        if (groups >= 2) {
            seq_t prod = 1;
            for (int j = 0; j < groups; ++j)
                prod *= _divisor_count(curg[j]);
            *total += prod;
        }
        return;
    }
    uint64_t cur_exp = e[idx];

    /* assign to existing groups */
    for (int j = 0; j < groups; ++j) {
        uint64_t old = curg[j];
        curg[j] = _gcd_u64(old, cur_exp);
        _dfs(idx + 1, groups, e, cnt, curg, total);
        curg[j] = old;  /* backtrack */
    }
    /* create new group */
    curg[groups] = cur_exp;
    _dfs(idx + 1, groups + 1, e, cnt, curg, total);
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* factor n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    if (cnt <= 1) return 0;    /* cannot have two distinct coprime factors */

    uint64_t exp_array[64];
    for (size_t i = 0; i < cnt; ++i) exp_array[i] = exps[i];

    seq_t total = 0;
    uint64_t curg[64] = {0};
    _dfs(0, 0, exp_array, (int)cnt, curg, &total);

    return total;
}
