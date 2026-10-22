/*
 * A390768  Binary length of A139391(n) - binary length of n.
 * https://oeis.org/A390768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Binary length of A139391(n) – Binary length of n. Here
 *            A139391(n) is the reduced Collatz step: • If n is even,
 *            A139391(n) = odd part of n (n >> v2(n)). • If n is odd,
 *            A139391(n) = (3*n + 1) / 2^e where e = v2(3*n + 1)
 * Range:     i8 n <= 42, i16 n <= 9316, i32 n <= 9316, i64 n <= 9316,
 *            i128 n <= 9316, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* even n */
        /* compute v2(n) */
        idx_t v2 = 0;
        idx_t tmp = n;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++v2;
        }
        seq_t next = (seq_t)(n >> v2); /* odd part of n */
        unsigned len_next = bitlen_seq_t(next);
        unsigned len_n    = bitlen_seq_t((seq_t)n);
        return (seq_t)len_next - (seq_t)len_n;
    } else {                           /* odd n */
        seq_t n_seq = (seq_t)n;
        seq_t val   = n_seq * 3 + 1;   /* 3*n + 1 */
        /* strip all trailing zeros (divide by 2^e) */
        while ((val & 1) == 0) {
            val >>= 1;
        }
        unsigned len_val = bitlen_seq_t(val);
        unsigned len_n   = bitlen_seq_t(n_seq);
        return (seq_t)len_val - (seq_t)len_n;
    }
}
