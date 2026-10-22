/*
 * A372696  For n>1, if n mod a(n-1) = 0 or a(n-1) mod n = 0, set a(n) = n +
 *          a(n-1); otherwise a(n) = abs(n - a(n-1)). Start with a(1)=1.
 * https://oeis.org/A372696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;  
   For n > 1, let a = a(n-1).  
   If n % a == 0 or a % n == 0, then a(n) = n + a;  
   otherwise a(n) = |n – a|. */
seq_t a(idx_t n)
{
    seq_t a_val = 1;                         /* a(1) */
    if (n <= 1) return a_val;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_val = (seq_t)i;

        if (i_val % a_val == 0 || a_val % i_val == 0) {
            a_val = a_val + i_val;          /* a(n) = n + a(n-1) */
        } else {
            a_val = (a_val > i_val) ? a_val - i_val : i_val - a_val; /* |n-a| */
        }
    }
    return a_val;
}
