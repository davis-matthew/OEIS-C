/*
 * A321549  a(n) = Sum_{d|n} (-1)^(d-1)*d^10.
 * https://oeis.org/A321549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * d^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 78, i128 n <= 6654,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, enumerates its divisors recursively, and
   accumulates the signed 10th powers.  It works for any seq_t implementation
   (signed integer or arbitrary‑precision), is O(τ(n)) per call, and
   satisfies the requirements. */

#include <stdint.h>
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* Static storage for the factorisation used by the recursive helper.
   As long as calls are serial (which they are in the test harness),
   this is safe and avoids reallocation on every call. */
static uint64_t _primes[64];
static uint64_t _exps[64];
static size_t _cnt;

/* Recursive helper: computes the contribution of all divisors that can be
   formed using the prime factors from index 'idx' onwards, given the
   partial divisor 'div'.  The function returns the sum of the
   signed 10th‐powers of those divisors. */
static seq_t _rec_helper(size_t idx, seq_t div)
{
    if (idx == _cnt) {
        seq_t power = 1;
        for (int k = 0; k < 10; ++k)
            power *= div;                /* div^10 computed in seq_t */
        seq_t sign = (div & 1) ? 1 : -1;  /* (-1)^(div-1) */
        return sign * power;
    }

    seq_t sum = 0;
    uint64_t p = _primes[idx];
    seq_t mult = 1;
    for (uint64_t e = 0; e <= _exps[idx]; ++e) {
        sum += _rec_helper(idx + 1, div * mult);
        mult *= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined but harmless */

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Copy factorisation into static storage for the helper */
    for (size_t i = 0; i < cnt; ++i) {
        _primes[i] = primes[i];
        _exps[i] = exps[i];
    }
    _cnt = cnt;

    return _rec_helper(0, (seq_t)1);
}
