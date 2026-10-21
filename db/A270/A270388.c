/*
 * A270388  a(n) = A048739(n-2) mod n.
 * https://oeis.org/A270388
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (((1-sqrt(2))^n + (1+sqrt(2))^n - 2) / 4) mod n, n > 1
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is not defined – return zero */
    if (n < 2)
        return (seq_t)0;

    /* V_0 = V_1 = 0 (recurrence: V_n = 2*V_{n-1} + V_{n-2} + 1) */
    idx_t v_prev2 = 0;   /* V_{i-2} */
    idx_t v_prev1 = 0;   /* V_{i-1} */
    idx_t v_curr  = 0;   /* V_i              */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute V_i mod n safely using 128‑bit arithmetic to avoid overflow */
        __uint128_t tmp = (__uint128_t)v_prev1 * 2 + v_prev2 + 1;
        v_curr = (idx_t)(tmp % n);

        v_prev2 = v_prev1;
        v_prev1 = v_curr;
    }

    return (seq_t)v_curr;   /* a(n) = V_n mod n */
}
