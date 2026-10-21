/*
 * A253453  Number of (n+1) X (5+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 529*3^(n-3) + 1832*2^(n-1) + 10087 for n > 6 * a(1)=8501,
 *            a(2)=15220, a(3)=19254, a(4)=26831, a(5)=44242, a(6)=83003 *
 *            This implementation uses the provided utilities pow_int (to
 *            compute powers) * and mul_small (to multiply a seq_t by a small
 *            integer)
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   mul_small, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"
#include "../utils/mul_small.h"

/* Return the n-th term of OEIS A253453.  n is 1‑based. */
seq_t a(idx_t n) {
    /* Pre‑computed first six terms */
    static const seq_t first[6] = {
        (seq_t)8501,  /* a(1) */
        (seq_t)15220, /* a(2) */
        (seq_t)19254, /* a(3) */
        (seq_t)26831, /* a(4) */
        (seq_t)44242, /* a(5) */
        (seq_t)83003  /* a(6) */
    };

    /* Handle tiny n quickly */
    if (n <= 6) {
        /* n is uint64_t, so n-1 is safe */
        return first[(idx_t)(n-1)];
    }

    /* Compute the exact formula for n > 6 */
    seq_t p3 = pow_int((seq_t)3, (idx_t)(n-3));   /* 3^(n-3)   */
    seq_t p2 = pow_int((seq_t)2, (idx_t)(n-1));   /* 2^(n-1)   */

    seq_t t1 = mul_small(p3, 529);    /* 529 * 3^(n-3) */
    seq_t t2 = mul_small(p2, 1832);   /* 1832 * 2^(n-1) */

    /* 10087 is a constant that fits in any seq_t size */
    return t1 + t2 + (seq_t)10087;
}
