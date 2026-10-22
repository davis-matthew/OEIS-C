/*
 * A320861  Powers of 2 with initial digit 5.
 * https://oeis.org/A320861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute powers of 2 whose first decimal digit is 5.
   Uses the fractional part of k·log10(2): a power 2^k starts with 5
   iff  log10(5) ≤ { k·log10(2) } < log10(6)   ({} = fractional part). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"

/* Constants (high precision 80‑bit long double) */
static const long double LOG10_2 = 0.3010299956639811952137388947244930267682L;
static const long double LOG10_5 = 0.6989700043360188047862611052755069732318L;
static const long double LOG10_6 = 0.7781512503836436325087660096149481086552L;

seq_t a(idx_t n)
{
    if (n == 0)                 /* OEIS offset is 1; index 0 is out of range */
        return (seq_t)0;

    /* Memoisation state */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t   next_exp  = 1;    /* next exponent to test (k) */
    static long double frac = 0.0L; /* fractional part of (k-1)*log10(2) */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Update fractional part for current exponent */
        frac = fmodl(frac + LOG10_2, 1.0L);

        /* Does 2^next_exp start with 5? */
        if (frac >= LOG10_5 && frac < LOG10_6) {
            /* Ensure capacity of cache */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure – abort generation */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the power of two */
            cache[cache_len++] = pow2(next_exp);
        }
        ++next_exp;
    }

    return cache[n - 1];
}
