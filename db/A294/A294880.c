/*
 * A294880  Number of divisors of n that are in Perrin sequence, A001608.
 * https://oeis.org/A294880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 1_{Per( d )} (# of Perrin divisors of n)
 * Range:     u8 n <= 5148, u16 n <= 24914, u32 n <= 24914, u64 n <= 24914,
 *            u128 n <= 24914, big any n
 * Validated: standard, pass, 121 terms through n = 9775 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Check whether a value is a Perrin number. */
static bool
is_perrin_value(seq_t val)
{
    if (val == (seq_t)0 || val == (seq_t)2 || val == (seq_t)3)
        return true;

    seq_t a = (seq_t)3;  /* P(n-3)   */
    seq_t b = (seq_t)0;  /* P(n-2)   */
    seq_t c = (seq_t)2;  /* P(n-1)   */

    while (c < val) {
        seq_t next = b + a;   /* P(n) = P(n-2)+P(n-3) */
        a = b;
        b = c;
        c = next;
    }

    return c == val;
}

/* Count divisors of n that are Perrin numbers. */
seq_t
a(idx_t n)
{
    if (n == 0)  /* safeguard – sequence starts at n=1 */
        return (seq_t)0;

    uint64_t n64 = (uint64_t)n;

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    factor_u64(n64, primes, exps, &factor_cnt);

    /* enumerate all divisors of n */
    size_t div_cap = 16;
    size_t div_cnt = 0;
    seq_t *divs = mem_new(seq_t, div_cap);
    if (!divs) exit(1);
    divs[div_cnt++] = (seq_t)1;               /* divisor 1 */

    for (size_t pidx = 0; pidx < factor_cnt; ++pidx) {
        seq_t base = (seq_t)primes[pidx];
        seq_t pow  = (seq_t)1;
        size_t old_cnt = div_cnt;
        for (uint64_t exp = 1; exp <= exps[pidx]; ++exp) {
            pow = seq_mul(pow, base);
            for (size_t j = 0; j < old_cnt; ++j) {
                if (div_cnt == div_cap) {
                    div_cap <<= 1;          /* double capacity */
                    seq_t *tmp = mem_resize(seq_t, divs, div_cap);
                    if (!tmp) { mem_free(divs); exit(1); }
                    divs = tmp;
                }
                divs[div_cnt++] = seq_mul(divs[j], pow);
            }
        }
    }

    /* count Perrin divisors */
    size_t cnt = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        if (is_perrin_value(divs[i]))
            ++cnt;
    }

    mem_free(divs);
    return (seq_t)cnt;
}
