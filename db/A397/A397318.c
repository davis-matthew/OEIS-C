/*
 * A397318  a(n) is the number of positive integers that are less than or
 *          equal to and relatively prime to exactly two divisors of n.
 * https://oeis.org/A397318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 304, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of x with 1 ≤ x ≤ n such that
 *  there are exactly two divisors d of n for which
 *        x ≤ d  and  gcd(x,d) = 1.
 *
 *  The implementation enumerates all divisors of n
 *  and, for each divisor d, increments a counter
 *  for every x ≤ d that is coprime to d.
 *  Finally it returns the number of x with counter = 2.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* only defined for n ≥ 1 */

    uint64_t N = n;                      /* local copy, unsigned */

    /* collect all divisors of N */
    size_t divcap = 16;
    uint64_t *divs = (uint64_t *)malloc(divcap * sizeof(uint64_t));
    size_t divc  = 0;

    for (uint64_t d = 1; d <= N / d; ++d) {
        if (N % d == 0) {
            if (divc == divcap) {
                divcap <<= 1;
                divs = (uint64_t *)realloc(divs, divcap * sizeof(uint64_t));
            }
            divs[divc++] = d;
            uint64_t e = N / d;
            if (e != d) {
                if (divc == divcap) {
                    divcap <<= 1;
                    divs = (uint64_t *)realloc(divs, divcap * sizeof(uint64_t));
                }
                divs[divc++] = e;
            }
        }
    }

    /* count, for each x, how many divisors satisfy the condition */
    uint8_t *cnt = (uint8_t *)calloc((size_t)N + 1, sizeof(uint8_t));

    for (size_t i = 0; i < divc; ++i) {
        uint64_t d = divs[i];
        for (uint64_t x = 1; x <= d; ++x) {
            if (gcd((seq_t)x, (seq_t)d) == (seq_t)1) {
                ++cnt[(size_t)x];
            }
        }
    }

    /* tally x with exactly two suitable divisors */
    seq_t result = 0;
    for (uint64_t x = 1; x <= N; ++x) {
        if (cnt[(size_t)x] == 2) {
            ++result;
        }
    }

    free(cnt);
    free(divs);
    return result;
}
