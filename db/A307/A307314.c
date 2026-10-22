/*
 * A307314  Number of divisors d of 2n such that adding d to 2n in binary
 *          requires no carries.
 * https://oeis.org/A307314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 2n such that (d & 2n) == 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Simple trial‑division factorisation for 64‑bit unsigned integers      */
/* --------------------------------------------------------------------- */
static void factor_uint64(uint64_t n,
                          uint64_t *primes,
                          uint64_t *exps,
                          size_t *cnt_out)
{
    size_t cnt = 0;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        primes[cnt] = 2ULL;
        exps[cnt]   = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exps[cnt]++;
        }
        cnt++;
    }

    /* odd factors */
    for (uint64_t p = 3ULL; p <= n / p; p += 2ULL) {
        if (n % p == 0) {
            primes[cnt] = p;
            exps[cnt]   = 0;
            while (n % p == 0) {
                n /= p;
                exps[cnt]++;
            }
            cnt++;
        }
    }

    /* remaining prime */
    if (n > 1ULL) {
        primes[cnt] = n;
        exps[cnt]   = 1;
        cnt++;
    }

    *cnt_out = cnt;
}

/* --------------------------------------------------------------------- */
/* Recursive divisor generator                                           */
/* --------------------------------------------------------------------- */
static void gen_div(uint64_t idx,
                    uint64_t cur,
                    const uint64_t *primes,
                    const uint64_t *exps,
                    size_t cnt,
                    uint64_t m,
                    uint64_t *count)
{
    if (idx == cnt) {
        if ((cur & m) == 0ULL) {
            (*count)++;
        }
        return;
    }

    /* e = 0 case */
    gen_div(idx + 1, cur, primes, exps, cnt, m, count);

    /* e >= 1 cases */
    uint64_t p   = primes[idx];
    uint64_t val = cur;
    for (uint64_t e = 1; e <= exps[idx]; e++) {
        if (val > m / p) break;   /* prevent overflow, though unlikely here */
        val *= p;
        gen_div(idx + 1, val, primes, exps, cnt, m, count);
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0U;   /* behaviour for n < 1 is arbitrary */

    uint64_t n64  = (uint64_t)n;
    uint64_t m    = n64 << 1;       /* 2 * n */

    /* factorise m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_uint64(m, primes, exps, &cnt);

    /* generate divisors and count those with no common 1‑bits */
    uint64_t count = 0;
    gen_div(0, 1ULL, primes, exps, cnt, m, &count);

    return (seq_t)count;
}
