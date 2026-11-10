/*
 * A384794  a(n) is the index of A384793(n) in A038618.
 * https://oeis.org/A384794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index in A038618 of the first term of the first maximal
 *            run of exactly n consecutive primes none of which contains digit
 *            0; computed by a search over primes
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

#define A384794_MAXN 4096

static bool A384794_has_zero(uint64_t x)
{
    while (x != 0) {
        if (x % 10 == 0) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t first[A384794_MAXN];   /* 0 = not yet found */
    static uint64_t x = 1;                 /* last number examined */
    static uint64_t count = 0;             /* zero-free primes seen so far */
    static uint64_t run_len = 0;
    static uint64_t run_start = 0;

    if (n == 0 || n >= A384794_MAXN) return (seq_t)0;

    while (first[n] == 0) {
        /* advance to next prime */
        if (x < 2) x = 2;
        else if (x == 2) x = 3;
        else x += 2;
        while (!is_prime_u64(x)) x += 2;

        if (!A384794_has_zero(x)) {
            count++;
            if (run_len == 0) run_start = count;
            run_len++;
        } else {
            if (run_len > 0 && run_len < A384794_MAXN && first[run_len] == 0)
                first[run_len] = run_start;
            run_len = 0;
        }
    }
    return (seq_t)first[n];
}
