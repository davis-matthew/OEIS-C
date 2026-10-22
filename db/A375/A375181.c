/*
 * A375181  Inverse binomial transform of A376277.
 * https://oeis.org/A375181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ((7*n-25)*a(n-1)+(-12*n+50)*a(n-2)+(-10*n+40)*a(n-3)+ *
 *            (32*n-146)*a(n-4)+(n-5)*a(n-5)+(-21*n+105)*a(n-6))/(n-3), * for
 *            n>=6. * Initial terms: * a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6,
 *            a(5)=12
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the n-th term of OEIS sequence A375181. */
seq_t a(idx_t n)
{
    /* Constants for first six terms. */
    static const seq_t init[6] = {1, 1, 2, 3, 6, 12};

    /* If n is within the initial range, return the constant. */
    if (n < 6) {
        return init[n];
    }

    /* Circular buffer holding the last six computed values. */
    seq_t buf[6];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];
    buf[4] = init[4];
    buf[5] = init[5];

    /* Compute terms from 6 up to n. */
    for (idx_t i = 6; i <= n; ++i) {
        /* Pre‑computed coefficients (always non‑negative by definition). */
        uint64_t c1 = 7 * i - 25;    /* coefficient of a(i-1)   */
        uint64_t c2 = 12 * i - 50;   /* coefficient of a(i-2)   */
        uint64_t c3 = 10 * i - 40;   /* coefficient of a(i-3)   */
        uint64_t c4 = 32 * i - 146;  /* coefficient of a(i-4)   */
        uint64_t c5 = i - 5;         /* coefficient of a(i-5)   */
        uint64_t c6 = 21 * i - 105;  /* coefficient of a(i-6)   */

        /* Access the required previous terms. */
        seq_t a1 = buf[(i - 1) % 6];
        seq_t a2 = buf[(i - 2) % 6];
        seq_t a3 = buf[(i - 3) % 6];
        seq_t a4 = buf[(i - 4) % 6];
        seq_t a5 = buf[(i - 5) % 6];
        seq_t a6 = buf[(i - 6) % 6];

#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
        /* Use 128‑bit arithmetic for C (guaranteed to exist on GCC/Clang). */
        __uint128_t sum_pos = ( __uint128_t ) a1 * c1
                            + ( __uint128_t ) a4 * c4
                            + ( __uint128_t ) a5 * c5;
        __uint128_t sum_neg = ( __uint128_t ) a2 * c2
                            + ( __uint128_t ) a3 * c3
                            + ( __uint128_t ) a6 * c6;
        __uint128_t rest    = sum_pos - sum_neg;
        seq_t ai            = (seq_t) (rest / (i - 3));
#else
        /* C++ or no 128‑bit support: rely on the `seq_t` arithmetic. */
        seq_t sum_pos = a1 * c1 + a4 * c4 + a5 * c5;
        seq_t sum_neg = a2 * c2 + a3 * c3 + a6 * c6;
        seq_t rest    = sum_pos - sum_neg;
        seq_t ai      = rest / (i - 3);
#endif

        /* Store the newly computed term. */
        buf[i % 6] = ai;
    }

    /* The result for index n is at position n % 6 in the buffer. */
    return buf[n % 6];
}
