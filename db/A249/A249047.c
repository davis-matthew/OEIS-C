/*
 * A249047  Self-numbers (A003052) that are not divisible by 3.
 * https://oeis.org/A249047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249047: Self‑numbers (A003052) that are NOT divisible by 3.
   A(n) = the n‑th integer that is not equal to k + sumdigits(k) for any k,
   and that is not a multiple of 3.  The function generates numbers on
   demand; a sieve of size O(n) is sufficient for all practical values. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t digit_sum_idx(idx_t x) {
    idx_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n = 0 – return 0 */

    /* Initial estimate for the maximum value to sieve.  
       Empirically about 20 × n is more than enough for n ≤ 10 000. */
    unsigned __int128 tmp = (unsigned __int128)n * 20 + 100;
    idx_t limit = (idx_t)tmp;

    while (true) {
        /* Allocate a bit vector to mark generated numbers. */
        bool *generated = (bool *)calloc((size_t)limit + 1, sizeof(bool));
        if (!generated) abort();

        /* Mark all numbers of the form m + sumdigits(m). */
        for (idx_t m = 1; m <= limit; ++m) {
            idx_t g = m + digit_sum_idx(m);
            if (g <= limit) generated[g] = true;
        }

        /* Scan for self‑numbers not divisible by 3. */
        idx_t count = 0;
        for (idx_t v = 1; v <= limit; ++v) {
            if (!generated[v] && (v % 3 != 0)) {
                ++count;
                if (count == n) {
                    free(generated);
                    return (seq_t)v;
                }
            }
        }

        /* Not enough terms found – double the sieve size and retry. */
        free(generated);
        if ((unsigned __int128)limit > ((unsigned __int128)~(idx_t)0) / 2)
            limit = ~(idx_t)0;           /* cap at maximum idx_t */
        else
            limit *= 2;
    }
}
