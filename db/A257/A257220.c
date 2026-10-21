/*
 * A257220  Numbers that have at least one divisor containing the digit 3 in
 *          base 10.
 * https://oeis.org/A257220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_contains_3, factor_u64, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This routine computes the OEIS sequence A257220:
 * numbers that have at least one divisor containing the digit 3 in decimal.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_contains_3.h"

/* recursive divisor generator; stops once a divisor that contains a 3 is found */
static bool gen_divs(const uint64_t *pr, const uint64_t *ex,
                     size_t idx, size_t cnt, seq_t cur)
{
    if (idx == cnt) {
        return digit_contains_3(cur);
    }

    seq_t val = cur;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        if (digit_contains_3(val))
            return true;
        if (gen_divs(pr, ex, idx + 1, cnt, val))
            return true;
        if (e < ex[idx])
            val *= (seq_t)pr[idx];
    }
    return false;
}

/* return true iff k has a divisor whose decimal representation contains a 3 */
static bool has_digit3_divisor(idx_t k)
{
    /* the number itself is a divisor */
    if (digit_contains_3((seq_t)k))
        return true;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    if (cnt == 0)          /* k == 1 */
        return false;

    return gen_divs(primes, exps, 0, cnt, (seq_t)1);
}

/* static cache */
static seq_t *cache   = NULL;
static size_t cache_cap   = 0;
static idx_t   cached_last = 0;   /* number of cached terms   */
static idx_t   next_val     = 1;   /* next integer to test */

/* public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset 1 – undefined for n==0 */

    /* return cached value if available */
    if (n <= cached_last)
        return cache[n - 1];

    /* generate terms until we reach n */
    while (cached_last < n) {
        /* find next integer whose divisors contain a 3 */
        while (!has_digit3_divisor(next_val))
            ++next_val;

        /* ensure capacity */
        if (cached_last == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp;
            if (cache == NULL) {
                tmp = seq_alloc(new_cap);
            } else {
                tmp = seq_realloc(cache, cache_cap, new_cap);
            }
            if (!tmp) /* allocation failure */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cached_last++] = (seq_t)next_val;
        ++next_val;
    }

    return cache[n - 1];
}
