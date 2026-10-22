/*
 * A328458  Maximum run-length of the nontrivial divisors (greater than 1 and
 *          less than n) of n.
 * https://oeis.org/A328458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum run-length of the nontrivial divisors of n
 *            (consecutive integers among divisors >1 and <n). By convention
 *            a(1)=1, a(p)=0 for primes
 * Range:     u8 n <= 7983, u16 n <= 7983, u32 n <= 7983, u64 n <= 7983,
 *            u128 n <= 7983, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static int cmp_idx(const void *a, const void *b)
{
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    return (x < y) ? -1 : (x > y ? 1 : 0);
}

seq_t a(idx_t n)
{
    /* special cases */
    if (n == 1)
        return (seq_t)1;

    /* -------------------------------------------------------------
       1. Factor n into primes with exponents.
       ------------------------------------------------------------- */
    const size_t PRIME_CAP = 64;               /* sufficient for uint64_t */
    uint64_t primes[PRIME_CAP];
    uint64_t exps[PRIME_CAP];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If n is prime (cnt==1 and exponent==1) there are no non‑trivial divisors. */
    if (cnt == 1 && exps[0] == 1)
        return (seq_t)0;

    /* -------------------------------------------------------------
       2. Generate all divisors (including 1 and n).
       ------------------------------------------------------------- */
    size_t div_cap = 16;
    idx_t *divs = (idx_t *)malloc(div_cap * sizeof(idx_t));
    if (!divs)
        return (seq_t)0;          /* allocation failure → 0 */
    size_t div_len = 1;
    divs[0] = 1;                 /* 1 is always a divisor */

    for (size_t pi = 0; pi < cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        uint64_t e = exps[pi];
        size_t cur_len = div_len;

        for (size_t j = 0; j < cur_len; ++j) {
            idx_t base = divs[j];
            idx_t val = base;
            for (uint64_t k = 1; k <= e; ++k) {
                val *= p;                /* val = base * p^k */
                if (div_len == div_cap) {
                    div_cap <<= 1;
                    divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                    if (!divs)
                        return (seq_t)0;  /* allocation failure → 0 */
                }
                divs[div_len++] = val;
            }
        }
    }

    /* -------------------------------------------------------------
       3. Keep only non‑trivial divisors ( >1 and < n ).
       ------------------------------------------------------------- */
    size_t fcap = div_len;
    idx_t *filtered = (idx_t *)malloc(fcap * sizeof(idx_t));
    if (!filtered) {
        free(divs);
        return (seq_t)0;
    }
    size_t flen = 0;

    for (size_t i = 0; i < div_len; ++i) {
        idx_t d = divs[i];
        if (d > 1 && d < n) {
            if (flen == fcap) {
                fcap <<= 1;
                filtered = (idx_t *)realloc(filtered, fcap * sizeof(idx_t));
                if (!filtered) {
                    free(divs);
                    return (seq_t)0;
                }
            }
            filtered[flen++] = d;
        }
    }
    free(divs);

    if (flen == 0)          /* should not happen except for primes already handled */
        return (seq_t)0;

    /* -------------------------------------------------------------
       4. Sort the divisors.
       ------------------------------------------------------------- */
    qsort(filtered, flen, sizeof(idx_t), cmp_idx);

    /* -------------------------------------------------------------
       5. Find the longest run of consecutive integers.
       ------------------------------------------------------------- */
    seq_t max_run = 1;
    seq_t cur_run = 1;
    for (size_t i = 1; i < flen; ++i) {
        if (filtered[i] == filtered[i - 1] + 1) {
            ++cur_run;
            if (cur_run > max_run)
                max_run = cur_run;
        } else {
            cur_run = 1;
        }
    }

    free(filtered);
    return max_run;
}
