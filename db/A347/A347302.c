/*
 * A347302  a(n) = 3^n - lcm{1..n}, with a(0) = 0.
 * https://oeis.org/A347302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n - lcm{1..n}
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 924 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_idx, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_idx.h"
#include "../utils/pow3.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static uint64_t *primes = NULL;    /* cached prime list */
static size_t primes_cnt = 0;       /* number of primes cached */
static idx_t  primes_limit = 0;    /* maximum n for which primes are cached */

static void
ensure_primes_upto(idx_t n)
{
    if (n <= primes_limit) return;
    free(primes);
    build_primes_upto(n, &primes, &primes_cnt);
    primes_limit = n;
}

/* Compute a(n) = 3^n - lcm(1..n) */
seq_t
a(idx_t n)
{
    ensure_primes_upto(n);

    /* LCM(1..n) = product over all primes p <= n of p^⌊log_p n⌋ */
    seq_t lcm = 1;
    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        idx_t  exp = 0;
        idx_t  val = p;
        /* find largest power of p <= n */
        while (val <= n) {
            ++exp;
            if (val > n / p) break;
            val *= p;
        }
        seq_t pow = ipow_idx((seq_t)p, exp);
        lcm = seq_mul(lcm, pow);
    }

    seq_t pow3n = pow3(n);          /* 3^n, defined in utils/pow3.h */
    return pow3n - lcm;
}
