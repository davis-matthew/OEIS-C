/*
 * A280645  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 5.
 * https://oeis.org/A280645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k^3 has an odd number of digits and the middle digit is 5 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Static cache of generated terms */
    static seq_t *cache = NULL;
    static size_t cap = 0, size = 0;
    static idx_t next_k = 1;   /* smallest k yet to be tested */

    /* Generate terms until we have at least n entries */
    while (size < n) {
        seq_t cube = (seq_t)next_k;
        cube = cube * next_k * next_k;   /* next_k^3 */

        int d = digit_len(cube);         /* number of decimal digits of cube */
        if (d & 1) {                     /* odd number of digits? */
            int exp = d - 1 - (d / 2);   /* exponent for 10^... to isolate middle digit */
            seq_t pow10val = pow10(exp);
            seq_t mid = (cube / pow10val) % 10;
            if (mid == 5) {
                append_candidate(&cache, &size, &cap, (seq_t)next_k);
            }
        }
        ++next_k;
    }
    return cache[n - 1];
}
