/*
 * A245385  Numbers N such that N = P//Q = R//S, where // is the concatenation
 *          function, satisfying the following properties: P and S are m-digit
 *          integers, Q and R are k-digit integers, k and m are distinct
 *          positive integers, and P*Q = R*S.
 * https://oeis.org/A245385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search: for each integer N we try all
 * distinct splits into m‑digit and k‑digit parts (k != m).  If
 * P*Q == R*S for any such split, N is a sequence term.  Terms are
 * cached in ascending order, so repeated calls for any n are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* seq_t, idx_t are defined by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    static seq_t *cache = NULL;    /* sorted valid numbers */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static idx_t last_N = 0;       /* last integer examined */

    /* enlarge cache when needed */
    while ((size_t)n > cache_len)
    {
        ++last_N;
        seq_t N = (seq_t)last_N;

        int d = digit_len(N);           /* number of decimal digits */

        bool ok = false;
        for (int k = 1; k < d && !ok; ++k)
        {
            int m = d - k;
            if (k == m) continue;       /* lengths must be distinct */

            /* 10^k and 10^m */
            seq_t powk = pow10((idx_t)k);
            seq_t powm = pow10((idx_t)m);

            seq_t P = N / powk;            /* first m‑digit part   */
            seq_t Q = N % powk;            /* last k‑digit part    */
            seq_t R = N / powm;            /* first k‑digit part   */
            seq_t S = N % powm;            /* last m‑digit part    */

            /* leading zero check: parts must have the required digits */
            seq_t minP = powm / 10;        /* 10^(m-1) */
            seq_t minQ = powk / 10;        /* 10^(k-1) */
            seq_t minR = powk / 10;
            seq_t minS = powm / 10;

            if (P < minP || Q < minQ || R < minR || S < minS) continue;

            if (P * Q == R * S) {
                ok = true;
            }
        }

        if (ok) {
            /* store the term */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = N;
        }
    }

    return cache[n - 1];
}
