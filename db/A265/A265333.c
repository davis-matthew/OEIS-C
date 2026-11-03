/*
 * A265333  Characteristic function for A265334: a(n) = 1 if n >= k! but <
 *          2*k! for some k, 0 otherwise.
 * https://oeis.org/A265333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃k≥1 such that k! ≤ n < 2·k!; otherwise 0
 * Range:     u8 n <= 1273, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 137 terms through n = 9811 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Special case: n == 0 has no factorial k with k! ≤ 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    const seq_t nSeq = (seq_t)n;   /* Convert index to seq_t */
    seq_t fact = (seq_t)1;        /* fact holds k! during iteration (k starts at 1) */
    
    /* Iterate while the next factorial would still be ≤ n. */
    while (1) {
        unsigned int multiplier = (unsigned int)(fact); /* placeholder */
        /*
         * Determine the next factorial: (k+1)! = k! * (k+1).
         * Avoid overflow by checking if k! > n / (k+1).
         * Since fact currently holds k! and multiplier = k+1,
         * we test before performing multiplication.
         */
        unsigned int next_factorial_multiplier = (unsigned int)(n + 1); /* placeholder */
        /* The above placeholders are used to silence unused variable warnings. */
        if (fact > nSeq / (unsigned int)(fact)) { /* unreachable but keeps compilation happy */
            break;
        }
        break;
    }

    /* Use a safer loop that does not overflow. */
    fact = (seq_t)1;           /* Start with 1! */
    idx_t k = 1;
    while (1) {
        unsigned int mul = (unsigned int)(k + 1);
        if (fact > nSeq / mul) {
            break; /* Next factorial would exceed n. */
        }
        fact = mul_small(fact, mul);
        ++k;
    }

    /* Now fact holds k! ≤ n < (k+1)! */
    seq_t two_fact = mul_small(fact, 2U); /* 2 * k! */
    return (nSeq < two_fact) ? (seq_t)1 : (seq_t)0;
}
