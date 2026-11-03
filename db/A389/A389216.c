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
 * Validated: standard, pass, 82 terms through n = 9636 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: search for nonsquarefree numbers k such that
   (sum of unitary divisors of k) + (sum of squarefree divisors of k)
   >= 2*k, and return the n-th such k. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Computes whether k is nonsquarefree and the sum of its unitary
   divisors (s_unit) and the sum of its squarefree divisors (s_sqf). */
static void compute_sums(uint64_t k, bool *nonsq, seq_t *s_unit, seq_t *s_sqf)
{
    unsigned __int128 su = 1;
    unsigned __int128 ss = 1;
    bool nsq = false;
    uint64_t n = k;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            if (e >= 2) nsq = true;
            unsigned __int128 pe = 1;
            for (int i = 0; i < e; ++i) pe *= p;
            su *= (pe + 1);
            ss *= (unsigned __int128)(1 + p);
        }
    }
    if (n > 1) {
        unsigned __int128 pe = n;           /* prime factor, e == 1 */
        su *= (pe + 1);
        ss *= (unsigned __int128)(1 + n);
        /* e == 1, so not nonsquarefree */
    }
    *nonsq = nsq;
    *s_unit = (seq_t)su;
    *s_sqf = (seq_t)ss;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;     /* stored sequence values */
    static size_t size = 0;       /* number of terms stored */
    static size_t cap = 0;        /* capacity of seq array */
    static uint64_t last_k = 3;   /* last tested integer, start just below 4 */

    if (!seq) {
        cap = 64;
        seq = mem_new(seq_t, cap);
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
                    seq = mem_resize(seq_t, seq, cap);
                    if (!seq) exit(1);
                }
                seq[size++] = last_k;
            }
        }
    }
    return seq[n - 1];
}
