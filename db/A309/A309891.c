/*
 * A309891  a(n) is the total number of trailing zeros in the representations
 *          of n over all bases b >= 2.
 * https://oeis.org/A309891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{d|n, d>1} v_d(n) (d‑adic valuation of n)
 * Range:     u8 n <= 8640, u16 n <= 8640, u32 n <= 8640, u64 n <= 8640,
 *            u128 n <= 8640, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Recursive enumeration of all divisors of n from its prime factorisation.
   `exps` holds the exponents of the prime factors of the original number `n`.
   `cur_f` is the exponents of the currently built divisor.  `sum` accumulates
   the total sum of valuations. */
static void dfs_divs(size_t pos, size_t cnt, const uint64_t *exps,
                     uint64_t *cur_f, seq_t *sum)
{
    if (pos == cnt) {
        /* divisor > 1 if at least one exponent is non‑zero.  Compute its
           largest power dividing n. */
        bool found = false;
        seq_t min_val = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (cur_f[i] > 0) {
                seq_t t = (seq_t)(exps[i] / cur_f[i]);  /* floor division */
                if (!found) {
                    min_val = t;
                    found   = true;
                } else if (t < min_val) {
                    min_val = t;
                }
            }
        }
        if (found)
            *sum += min_val;
        return;
    }

    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        cur_f[pos] = e;
        dfs_divs(pos + 1, cnt, exps, cur_f, sum);
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 1)
        return 0;

    /* Factorise N into primes with their exponents.  We use a simple
       trial‑division algorithm, which is fast enough for the expected
       values of n (≤ 10 000 in tests). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    uint64_t temp = N;

    if (temp % 2 == 0) {
        primes[cnt] = 2;
        uint64_t e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        exps[cnt++] = e;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes[cnt] = p;
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }

    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt++] = 1;
    }

    seq_t sum = 0;
    uint64_t cur_f[64] = {0};

    if (cnt > 0)
        dfs_divs(0, cnt, exps, cur_f, &sum);

    return sum;
}
