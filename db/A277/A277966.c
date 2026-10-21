/*
 * A277966  Numbers whose largest decimal digit is 4.
 * https://oeis.org/A277966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = nth positive integer whose largest decimal digit is 4.
 * The sequence is obtained by counting numbers with digits 0‑4 that contain
 * at least one 4.  The count of numbers with at most L digits equals
 * 5^L - 4^L.  We locate the appropriate digit length L and build the
 * nth number in lexicographic (hence numeric) order. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n = 0, guard */
    seq_t nn = (seq_t)n;

    /* Find length L such that 5^L - 4^L >= n */
    seq_t pow5 = 1, pow4 = 1;
    int L = 0;
    while (pow5 - pow4 < nn) {
        L++;
        pow5 *= 5;
        pow4 *= 4;
    }
    seq_t pow5_prev = pow5 / 5;
    seq_t pow4_prev = pow4 / 4;
    seq_t cum_before = pow5_prev - pow4_prev;
    seq_t k = nn - cum_before - 1;   /* zero‑based index within length L block */

    /* Precompute small powers of 5 and 4 up to the needed length */
    seq_t pow5_arr[L + 1];
    seq_t pow4_arr[L + 1];
    pow5_arr[0] = pow4_arr[0] = 1;
    for (int i = 1; i <= L; i++) {
        pow5_arr[i] = pow5_arr[i - 1] * 5;
        pow4_arr[i] = pow4_arr[i - 1] * 4;
    }

    seq_t res = 0;
    bool has4 = false;
    for (int pos = 0; pos < L; pos++) {
        int start = (pos == 0) ? 1 : 0;   /* leading digit cannot be 0 */
        int rem = L - 1 - pos;
        for (int d = start; d <= 4; d++) {
            bool next_has4 = has4 || (d == 4);
            seq_t count = next_has4 ? pow5_arr[rem] : (pow5_arr[rem] - pow4_arr[rem]);

            if (k < count) {
                res = res * 10 + (seq_t)d;
                has4 = next_has4;
                break;
            } else {
                k -= count;
            }
        }
    }
    return res;
}
