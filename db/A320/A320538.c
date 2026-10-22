/*
 * A320538  Assuming the truth of the Collatz conjecture, a(n) is the number
 *          of divisors of n appearing in the Collatz trajectory of n.
 * https://oeis.org/A320538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that appear in the Collatz *
 *            trajectory of n (assuming the Collatz conjecture holds)
 * Range:     u8 n <= 9883, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* 1.  Factorise n (handle n==1: no prime factors) */
    uint64_t N = (uint64_t)n;  /* n fits into 64‑bit for the required limits */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64(N, primes, exps, &pf_cnt);

    /* 2.  Compute number of divisors: product of (ei+1) */
    size_t dcnt = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        dcnt *= (size_t)(exps[i] + 1);

    /* 3.  Generate all divisors of N */
    uint64_t *divs = (uint64_t *)calloc(dcnt, sizeof(uint64_t));
    if (!divs) return 0;  /* unlikely for n≤10 000 */
    divs[0] = 1;
    size_t cur_len = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        int      e = (int)exps[i];
        size_t old_len = cur_len;
        uint64_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < old_len; ++j)
                divs[cur_len++] = divs[j] * p_pow;
        }
    }

    /* 4.  Scan the Collatz path and mark seen divisors */
    bool *seen = (bool *)calloc(dcnt, sizeof(bool));
    if (!seen) {
        free(divs);
        return 0;
    }

    uint64_t cur = N;
    for (;;) {
        /* Linear search – small divisor count keeps it fast */
        for (size_t i = 0; i < dcnt; ++i)
            if (cur == divs[i]) seen[i] = true;

        if (cur == 1) break;
        if ((cur & 1ULL) == 0)
            cur >>= 1;
        else
            cur = 3 * cur + 1;
    }

    /* 5.  Count divisors that appeared */
    seq_t count = 0;
    for (size_t i = 0; i < dcnt; ++i)
        if (seen[i]) ++count;

    /* 6.  Cleanup */
    free(divs);
    free(seen);
    return count;
}
