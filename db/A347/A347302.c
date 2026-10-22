/*
 * A347302  a(n) = 3^n - lcm{1..n}, with a(0) = 0.
 * https://oeis.org/A347302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n - lcm{1..n}
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   build_primes_upto, pow3, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"  /* build_primes_upto(limit, &primes, &cnt) */
#include "../utils/pow3.h"              /* seq_t pow3(idx_t exp) */
#include "../utils/seq_pow.h"           /* seq_t seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"           /* seq_t seq_mul(seq_t a, seq_t b) */

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
        seq_t pow = seq_pow((seq_t)p, exp);
        lcm = seq_mul(lcm, pow);
    }

    seq_t pow3n = pow3(n);          /* 3^n, defined in utils/pow3.h */
    return pow3n - lcm;
}
