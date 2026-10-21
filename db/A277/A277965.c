/*
 * A277965  Numbers whose largest decimal digit is 3.
 * https://oeis.org/A277965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277965: numbers whose largest decimal digit is 3.
   The nth term is the nth smallest positive integer whose maximum
   decimal digit equals 3.  The function computes this by combinatorial
   counting: the number of such d‑digit numbers is 4^d−3^d.  The algorithm
   determines the digit length of the n‑th term, then constructs the
   number digit by digit using the fact that at each position the
   remaining suffix must contain a 3 somewhere unless one has already
   appeared. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n must be ≥ 1; for n = 0 return 0 (undefined behaviour). */
    if (n == 0u)
        return 0u;

    /* Find the digit length d of a(n).  countPrev = total numbers with
       fewer than d digits.  All intermediate values fit in seq_t. */
    seq_t pow4_prev = 1u, pow3_prev = 1u;   /* 4^(d-1), 3^(d-1) */
    seq_t countPrev = 0u;                   /* 4^(d-1) - 3^(d-1) */
    int d = 0;
    seq_t pow4_cur, pow3_cur, countCur;

    seq_t n_seq = (seq_t)n;

    do {
        ++d;
        pow4_cur = pow4_prev * (seq_t)4;
        pow3_cur = pow3_prev * (seq_t)3;
        countCur = pow4_cur - pow3_cur;
        if (countCur >= n_seq)
            break;
        pow4_prev = pow4_cur;
        pow3_prev = pow3_cur;
        countPrev = countCur;
    } while (true);

    /* Rank within the d‑digit numbers (1‑based). */
    seq_t r = n_seq - countPrev;  /* 1 ≤ r ≤ countCur */

    /* Precompute powers of 4 and 3 up to d (array index = exponent). */
    static seq_t pow4_arr[128];
    static seq_t pow3_arr[128];
    pow4_arr[0] = 1u;
    pow3_arr[0] = 1u;
    for (int i = 1; i <= d; ++i) {
        pow4_arr[i] = pow4_arr[i - 1] * (seq_t)4;
        pow3_arr[i] = pow3_arr[i - 1] * (seq_t)3;
    }

    seq_t number = 0u;
    bool seen3 = false;

    for (int pos = 0; pos < d; ++pos) {
        int rem = d - pos - 1;      /* remaining positions */
        int start = (pos == 0) ? 1 : 0;   /* leading digit cannot be 0 */

        for (int digit = start; digit <= 3; ++digit) {
            bool has3After = seen3 || digit == 3;
            seq_t cnt = has3After
                        ? pow4_arr[rem]
                        : (pow4_arr[rem] - pow3_arr[rem]);

            if (r > cnt) {
                r -= cnt;
                continue;
            }

            number = number * (seq_t)10 + (seq_t)digit;
            seen3 = has3After;
            break;
        }
    }

    return number;
}
