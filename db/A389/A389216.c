/*
 * A389216  Nonsquarefree numbers k such that {sum of unitary divisors of k}
 *          plus {sum of squarefree divisors of k} >= 2*k.
 * https://oeis.org/A389216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 4186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: search for nonsquarefree numbers k such that
   (sum of unitary divisors of k) + (sum of squarefree divisors of k)
   >= 2*k, and return the n-th such k. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_sums.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;     /* stored sequence values */
    static size_t size = 0;       /* number of terms stored */
    static size_t cap = 0;        /* capacity of seq array */
    static uint64_t last_k = 3;   /* last tested integer, start just below 4 */

    if (!seq) {
        cap = 64;
        seq = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!seq) exit(1);
    }

    while (size < n) {
        ++last_k;                      /* candidate integer */

        /* compute sums and check nonsquarefree property */
        bool nonsq;
        seq_t s_unit, s_sqf;
        compute_sums(last_k, &nonsq, &s_unit, &s_sqf);

        if (nonsq) {
            unsigned __int128 lhs = (unsigned __int128)s_unit + (unsigned __int128)s_sqf;
            unsigned __int128 rhs = (unsigned __int128)last_k << 1;  /* 2*last_k */
            if (lhs >= rhs) {
                if (size == cap) {
                    cap <<= 1;
                    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                    if (!seq) exit(1);
                }
                seq[size++] = last_k;
            }
        }
    }
    return seq[n - 1];
}
