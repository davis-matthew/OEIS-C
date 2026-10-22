/*
 * A320988  Fibonacci rabbit sequence number n coded in base six.
 * https://oeis.org/A320988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(1) = 1 a(2) = 6 For n ≥ 3: a(n) = (6^{F_{n-1}}) *
 *            a(n-1) + a(n-2) where F_k is the k‑th Fibonacci number (F_0 = 0,
 *            F_1 = 1). The recurrence used below exploits 6^{F_{k+1}} =
 *            6^{F_k} * 6^{F_{k-1}}, allowing successive powers to be obtained
 *            via multiplication
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The function assumes that seq_t and idx_t are defined in the build. */
seq_t a(idx_t n) {
    /* Handle the initial terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    /* a_prev2 = a(n-2), a_prev1 = a(n-1) */
    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)6;   /* a(2) */

    /* Power variables:
       p_prevprev = 6^{F_{i-2}}, p_prev = 6^{F_{i-1}}
       Start with i = 3: F_1 = 1 and F_2 = 1. */
    seq_t p_prevprev = (seq_t)6; /* 6^{F_2} = 6^1 = 6 */
    seq_t p_prev    = (seq_t)6;  /* 6^{F_3} = 6^1 = 6 (same for i = 3) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t pow_cur;
        if (i == 3) {
            /* For the first step, the power is 6^{F_2} = 6 */
            pow_cur = p_prev;   /* 6 */
        } else {
            /* For i ≥ 4, use F_{i-1} = F_{i-2} + F_{i-3} */
            pow_cur = p_prev * p_prevprev;
        }

        /* a(i) = pow_cur * a_prev1 + a_prev2 */
        seq_t a_cur = pow_cur * a_prev1 + a_prev2;

        /* Shift values for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
        p_prevprev = p_prev;
        p_prev = pow_cur;
    }

    return a_prev1;
}
