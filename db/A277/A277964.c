/*
 * A277964  Numbers whose largest decimal digit is 2.
 * https://oeis.org/A277964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose decimal digits are * 0,1,2
 *            and contain at least one 2 (i.e. the largest decimal digit is 2)
 * Range:     u8 n <= 19, u16 n <= 211, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_L 64  /* sufficient for n up to at least 10^4 */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1, but undefined for n==0 */

    /* 1. Determine the number of decimal digits L of a(n). */
    idx_t L = 1;
    seq_t pow3_lminus1 = 1; /* 3^(L-1) */
    seq_t pow2_lminus1 = 1; /* 2^(L-1) */
    seq_t cum = 0;          /* total count of numbers with fewer than L digits */
    for (;;) {
        /* count for length L: 2*3^(L-1) - 2^(L-1) */
        seq_t cnt = (seq_t)2 * pow3_lminus1 - pow2_lminus1;
        if (cum + cnt >= n) break;
        cum += cnt;
        L++;
        pow3_lminus1 *= (seq_t)3;
        pow2_lminus1 *= (seq_t)2;
    }

    /* 2. Convert n into a 0‑based rank k among the numbers of length L. */
    seq_t k = (seq_t)(n - cum - 1);   /* 0‑based index within the block */

    /* 3. Pre‑compute powers of 3 and 2 up to exponent L. */
    seq_t pow3[MAX_L + 1], pow2[MAX_L + 1];
    pow3[0] = pow2[0] = 1;
    for (idx_t i = 1; i <= L; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }

    /* 4. Build the number digit‑by‑digit. */
    seq_t value = 0;
    bool seen2 = false;      /* Did we already place a digit 2? */
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1;                /* digits still to decide */
        int digits[3];
        int nd = 0;                               /* number of candidates */
        if (pos == 0) {                          /* most significant digit */
            digits[nd++] = 1;
            digits[nd++] = 2;
        } else {                                 /* subsequent digits */
            digits[nd++] = 0;
            digits[nd++] = 1;
            digits[nd++] = 2;
        }

        for (int ci = 0; ci < nd; ++ci) {
            int d = digits[ci];
            bool new_seen2 = seen2 || (d == 2);
            seq_t cnt;
            if (new_seen2) {
                cnt = pow3[rem];                /* any digits allowed */
            } else {
                cnt = pow3[rem] - pow2[rem];    /* must eventually place a 2 */
            }

            if (k < cnt) {
                /* this digit is chosen */
                value = value * (seq_t)10 + (seq_t)d;
                seen2 = new_seen2;
                break;
            } else {
                k -= cnt;
            }
        }
    }

    return value;
}
