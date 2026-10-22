/*
 * A332653  a(n) = (1/n) * Sum_{k=1..n} n^(k/gcd(n, k)).
 * https://oeis.org/A332653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Sum_{k=1..d, gcd(k,d)=1} n^(k-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the sequence A332653, usable with any seq_t type
 * (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
 * or an arbitrary‑precision integer class).  It caches previously
 * computed values in a static array so repeated calls are fast and
 * independent of the order of calls. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *memo = NULL;          /* cached values of a(n) */
static idx_t memo_cap = 0;          /* size of memo array */

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* the sequence is defined for n>=1 */
    /* Expand the memo array if necessary */
    if (n >= memo_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        /* initialise newly allocated slots to 0 */
        for (idx_t i = memo_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        memo = tmp;
        memo_cap = new_cap;
    }
    /* return cached value if already computed */
    if (memo[n] != (seq_t)0) return memo[n];

    /* pre‑compute n^e for e = 0 .. n   (pow[0] = 1) */
    seq_t *pow = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow) abort();
    pow[0] = (seq_t)1;
    seq_t base = (seq_t)n;
    for (idx_t e = 1; e <= n; ++e) pow[e] = pow[e-1] * base;

    seq_t sum = (seq_t)0;
    /* iterate over all divisors d of n */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        /* inner sum over k with gcd(k,d)=1 */
        for (idx_t k = 1; k <= d; ++k) {
            if (gcd((seq_t)k, (seq_t)d) == (seq_t)1)
                sum += pow[k-1];   /* n^(k-1) */
        }
    }

    free(pow);
    memo[n] = sum;                 /* cache result */
    return sum;
}
