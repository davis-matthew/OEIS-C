/*
 * A365512  a(n) is the least odd prime p such that A000120(n*p) = A000120(n)
 *          * A000120(p).
 * https://oeis.org/A365512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least odd prime p such that popcount(n*p) = popcount(n) *
 *            popcount(p)
 * Range:     u8 n <= 21, u16 n <= 283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Count the number of 1-bits in a value of type seq_t.
 * The result never exceeds 512 for the ranges considered here,
 * so an unsigned int is sufficient. */
static unsigned popcount_seq(seq_t x)
{
    unsigned cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    const seq_t sn = (seq_t)n;
    const unsigned pc_n = popcount_seq(sn);

    /* start with the smallest odd prime 3 */
    uint64_t p = 3;
    for (;;) {
        const unsigned pc_p = popcount_seq((seq_t)p);
        const seq_t prod = sn * (seq_t)p;
        const unsigned pc_prod = popcount_seq(prod);

        if (pc_prod == pc_n * pc_p)
            return (seq_t)p;

        p = next_prime(p);
    }
}
