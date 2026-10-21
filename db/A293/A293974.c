/*
 * A293974  Row sums of antidiagonals of the Sierpinski carpet A153490.
 * https://oeis.org/A293974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 336, u16 n <= 9852, u32 n <= 9852, u64 n <= 9852,
 *            u128 n <= 9852, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A293974: Row sums of antidiagonals of the Sierpinski carpet.
   Recurrence from the OEIS entry (a(0)=0):
     For k ≥ 0 with 3^k ≤ n < 3^(k+1):
       a(n) = 2·a(n‑3^k)   +   a(2·3^k‑n)          if 3^k < n ≤ 2·3^k
       a(n) = 2·a(n‑2·3^k) + 2·a(3·3^k‑n)          if 2·3^k < n < 3^(k+1)
       a(3^k)   = 2^k
       a(2·3^k) = 2^(k+1)
   a(1) = 1 (offset 1).
   The function is fully recursive and uses only integer arithmetic.
   No dynamic allocation or external helpers are needed. */

static seq_t rec(idx_t n)
{
    if (n == 0)                /* auxiliary base, never reached for n≥1 */
        return 0;
    /* find the largest power of 3 ≤ n */
    idx_t power = 1;
    int k = 0;
    while (power <= n / 3)
    {
        power *= 3;
        ++k;
    }
    /* exact power of 3 */
    if (n == power)
        return (seq_t)(1) << k;
    /* exact 2·3^k */
    if (n == 2 * power)
        return (seq_t)(1) << (k + 1);
    /* 3^k < n ≤ 2·3^k */
    if (n <= 2 * power)
        return (seq_t)(2) * rec(n - power) + rec(2 * power - n);
    /* 2·3^k < n < 3^(k+1) */
    return (seq_t)(2) * rec(n - 2 * power) + (seq_t)(2) * rec(3 * power - n);
}

seq_t a(idx_t n)
{
    /* The sequence uses offset 1; a(1) is the first term above.  */
    return rec(n);
}
