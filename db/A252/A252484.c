/*
 * A252484  Numbers m such that m^k is zeroless for k=1,...,4.
 * https://oeis.org/A252484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for   m   such that m, m^2, m^3, and m^4
   all have no digit 0 in their decimal representation.
   a(1) = 1, a(2) = 2, a(3) = 3, …
   The function works for any seq_t and idx_t defined by the
   build environment.  */
/* It uses a fixed-size cache of 20000 terms – enough for n ≤ 10000
   as required by the performance specification.  */
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/big_pow.h"

/* Return true iff x contains no zero digit in decimal form. */
static bool is_zeroless(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    return cnt[0] == 0;
}

seq_t a(idx_t n)
{
    /* fixed-size cache must be large enough for n ≤ 10000 */
    static seq_t cache[20000];
    static size_t len = 0;      /* number of terms stored so far */
    static idx_t curr = 0;      /* last integer examined (1‑based) */

    while (len < n) {
        curr++;                   /* candidate m = curr */
        seq_t m = (seq_t)curr;

        if (!is_zeroless(m))
            continue;            /* skip if m itself has a zero */

        /* check m^2, m^3, m^4 */
        bool ok = true;
        for (int k = 2; k <= 4 && ok; ++k) {
            seq_t p = big_pow(m, (seq_t)k);
            if (!is_zeroless(p))
                ok = false;
        }

        if (ok) {
            cache[len++] = m;
        }
    }

    return cache[n - 1]; /* 1‑based indexing */
}
