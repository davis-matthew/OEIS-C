/*
 * A258614  Numbers m having with the largest square <= m a common divisor >
 *          1.
 * https://oeis.org/A258614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, gcd, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258614: Numbers m having with the largest square <= m a common
 * divisor > 1.  A value m belongs to the sequence iff gcd(m,
 * floor_sqrt(m)) > 1.  The function evaluates terms on demand and caches
 * them so that calls with arbitrary n are fast. */
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq.h"

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    static size_t      sz   = 0;        /* number of terms stored          */
    static size_t      cap  = 0;        /* capacity of the array           */
    static seq_t      *arr  = NULL;     /* array of terms                  */
    static seq_t      nextm = 2;        /* next candidate integer to test  */

    while (sz < n) {
        seq_t m = nextm;
        seq_t k = floor_sqrt(m);

        if (gcd(m, k) > (seq_t)1) {
            if (sz == cap) {
                size_t new_cap = cap ? cap << 1 : 1;
                grow_seq(&arr, &cap, new_cap);
            }
            arr[sz++] = m;
        }
        ++nextm;
    }
    return arr[n - 1];
}
