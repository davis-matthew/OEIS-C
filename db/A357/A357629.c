/*
 * A357629  Half-alternating sum of the prime indices of n.
 * https://oeis.org/A357629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Half‑alternating sum of the prime indices of n.
   a(1) = 0; for n ≥ 2 a(n) = Σ_{i} (-1)^{⌊(i-1)/2⌋} * prime_index(p_i)
   where the multiset {p_i} consists of the prime indices of n repeated
   according to the multiplicity of each prime divisor of n. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* Cached prime list for repeated calls */
static uint64_t *prime_list = NULL;
static size_t      prime_cnt = 0;      /* number of primes in prime_list   */
static idx_t      max_prime_limit = 0; /* highest n for which primes are built */

/* Find the 1‑based index of prime p in prime_list via binary search.
   The prime list is monotonically increasing. */
static inline int64_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] == p)
            return (int64_t)(mid + 1); /* 1‑based index */
        else if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1; /* should never happen for a valid prime p */
}

seq_t
a(idx_t n)
{
    if (n <= 1)
        return 0; /* a(1) = 0 */

    /* Build or extend the prime list up to n once per call. */
    if (n > max_prime_limit) {
        if (prime_list != NULL)
            free((void *)prime_list);
        build_primes_upto(n, &prime_list, &prime_cnt);
        max_prime_limit = n;
    }

    /* Factor n and obtain primes and exponents. */
    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t   cnt_f = 0;
    factor_u64((uint64_t)n, primes_f, exps_f, &cnt_f);

    seq_t sum  = 0;
    idx_t pos  = 0; /* zero‑based position in the alternating pattern */

    for (idx_t i = 0; i < (idx_t)cnt_f; ++i) {
        uint64_t p  = primes_f[i];
        uint64_t exp = exps_f[i];
        int64_t  idx = prime_index(p); /* 1‑based index of p */

        if (idx <= 0)
            continue; /* defensive; should not happen */

        for (uint64_t k = 0; k < exp; ++k) {
            /* pattern: + + - - + + - - ... */
            if ((pos & 3) < 2)         /* 0 or 1 => add */
                sum += (seq_t)idx;
            else                        /* 2 or 3 => subtract */
                sum -= (seq_t)idx;
            ++pos;
        }
    }
    return sum;
}
