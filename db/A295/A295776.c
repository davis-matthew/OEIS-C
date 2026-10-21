/*
 * A295776  Number of n X 3 0..1 arrays with each 1 adjacent to 0 or 2
 *          king-move neighboring 1s.
 * https://oeis.org/A295776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)+6*a(n-3)-3*a(n-4)+2*a(n-5); Initial
 *            values: a(1)=5, a(2)=19, a(3)=56, a(4)=198, a(5)=665
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[5] = {
    (seq_t)5,
    (seq_t)19,
    (seq_t)56,
    (seq_t)198,
    (seq_t)665
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard against calls with n=0 */

    if (n <= 5) return init_vals[n - 1];

    /* ring buffer holding the last five terms: a(k-5) … a(k-1) */
    seq_t v[5] = {
        init_vals[0],  /* a(k-5) for k=6 initialised as a1          */
        init_vals[1],  /* a(k-4)                                */
        init_vals[2],  /* a(k-3)                                */
        init_vals[3],  /* a(k-2)                                */
        init_vals[4]   /* a(k-1)                                */
    };

    seq_t next;
    for (idx_t k = 6; k <= n; ++k) {
        next = 2 * v[4] + 3 * v[3] + 6 * v[2] - 3 * v[1] + 2 * v[0];

        /* rotate the buffer: drop oldest value, shift others, insert new */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }

    return v[4];   /* v[4] now holds a(n) */
}
