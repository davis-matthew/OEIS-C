/*
 * A323394  Carryless sum of divisors of n.
 * https://oeis.org/A323394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Carryless sum of divisors of n in base 10 (OEIS A323394)
 * Range:     u8 n <= 199, u16 n <= 9662, u32 n <= 9662, u64 n <= 9662,
 *            u128 n <= 9662, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* get all divisors of n via prime factorisation */
    uint64_t un = (uint64_t)n;
    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_u64(un, pr, ex, &cnt);

    /* dynamic array of divisors */
    size_t div_cap = 8;
    uint64_t *divs = (uint64_t*)malloc(div_cap * sizeof(uint64_t));
    divs[0] = 1;
    size_t div_len = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        size_t old_len = div_len;
        for (size_t j = 0; j < old_len; ++j) {
            uint64_t mult = 1;
            for (uint64_t k = 1; k <= e; ++k) {
                mult *= p;
                if (div_len == div_cap) {
                    div_cap *= 2;
                    divs = (uint64_t*)realloc(divs, div_cap * sizeof(uint64_t));
                }
                divs[div_len++] = divs[j] * mult;
            }
        }
    }

    /* carryless digit sums */
    int digits[25];
    memset(digits, 0, sizeof digits);

    for (size_t idx = 0; idx < div_len; ++idx) {
        uint64_t d = divs[idx];
        size_t pos = 0;
        while (d > 0) {
            digits[pos] = (digits[pos] + (int)(d % 10)) % 10;
            d /= 10;
            ++pos;
        }
    }

    /* convert digit array back to a number */
    seq_t res = 0;
    int high = (int)(sizeof(digits) / sizeof(digits[0])) - 1;
    while (high > 0 && digits[high] == 0) --high;
    for (int i = high; i >= 0; --i) {
        res = res * 10 + digits[i];
    }

    free(divs);
    return res;
}
