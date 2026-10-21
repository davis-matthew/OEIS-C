/*
 * A258996  Permutation of the positive integers: this permutation transforms
 *          the enumeration system of positive irreducible fractions
 *          A002487/A002487' (Calkin-Wilf) into the enumeration system
 *          A162911/A162912 (Drib), and vice versa.
 * https://oeis.org/A258996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   Permutation A258996 a(1)=1, a(2)=2, a(3)=3. For n = 2^m + k (m ≥
 *            1, 0 ≤ k < 2^m): if m is even: a(2^(m+1)+k) = a(2^m+k)+2^m
 *            a(2^(m+1)+2^m+k) = a(2^m+k)+2^(m+1) if m is odd: a(2^(m+1)+k) =
 *            a(2^m+k)+2^(m+1) a(2^(m+1)+2^m+k) = a(2^m+k)+2^m This
 *            implementation computes a(n) by repeatedly reducing n t...
 * Range:     u8 n <= 255, u16 n <= 9739, u32 n <= 9739, u64 n <= 9739,
 *            u128 n <= 9739, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 1;          /* a(1) = 1 */

    seq_t res = 1;                 /* start from a(1) */
    while (n > 1) {
        /* floor(log2(n)) */
        unsigned m = 63 - __builtin_clzll((unsigned long long)n);   /* 2^m ≤ n < 2^(m+1) */
        idx_t high   = (idx_t)1ULL << m;          /* 2^m      */
        idx_t half   = (idx_t)1ULL << (m - 1);    /* 2^(m-1)  */
        idx_t rem    = n - high;                  /* k        */

        idx_t base_index;
        seq_t  offset;

        if (rem < half) {                       /* first half: 2^m + k  */
            base_index = half + rem;            /* 2^(m-1) + k          */

            /* offset depends on parity of (m-1) */
            if ((m - 1) & 1)                    /* (m-1) odd → +2^m      */
                offset = (seq_t)(1ULL << m);
            else                                /* (m-1) even → +2^(m-1)  */
                offset = (seq_t)(1ULL << (m - 1));
        } else {                                /* second half: 2^m + 2^(m-1) + k */
            idx_t base_rem = rem - half;        /* k  in [0,2^(m-1)-1]   */
            base_index = half + base_rem;       /* 2^(m-1) + k          */

            if ((m - 1) & 1)                    /* (m-1) odd → +2^(m-1)  */
                offset = (seq_t)(1ULL << (m - 1));
            else                                /* (m-1) even → +2^m    */
                offset = (seq_t)(1ULL << m);
        }

        res += offset;
        n = base_index;
    }
    return res;
}
