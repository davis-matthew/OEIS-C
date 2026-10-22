/*
 * A356011  a(n) = n! * Sum_{k=1..n} 1/(k! * floor(n/k)).
 * https://oeis.org/A356011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} 1/(k! * floor(n/k))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterational computation using the recurrence:
   term_{k-1} = term_k * k * floor(n/k) / floor(n/(k-1))  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* value is not defined for n < 1 */
    seq_t sum   = 0;
    seq_t term  = 1;          /* term for k = n  →  n! / (n! * floor(n/n)) = 1     */
    sum += term;
    idx_t f_prev = n / n;     /* floor(n/n) == 1 */
    for (idx_t k = n; k > 1; --k) {
        /* floor(n/(k-1)) for the next term */
        idx_t f_next = n / (k - 1);
        /* Compute term_{k-1} carefully using 128‑bit arithmetic
           to avoid overflow whenever seq_t is a 64‑bit or 128‑bit
           unsigned integer type.               */
        unsigned __int128 temp =
            (unsigned __int128)term *
            (unsigned __int128)k *
            (unsigned __int128)f_prev;
        temp /= f_next;                   /* exact division */
        term = (seq_t)temp;
        sum += term;
        f_prev = f_next;
    }
    return sum;
}
