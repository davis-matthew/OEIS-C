/*
 * A260393  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (0,1) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=1.
   For n>2: let k be the smallest integer with L_k = 3·2^(k‑1)−1 ≥ n.
   Let center = (L_k+1)/2. Then
       if n == center      →  a(n) = k mod 2  (0 or 1)
       if n < center       →  a(n) = a(n) in the left half
       if n > center       →  a(n) = a(L_{k‑1} – (n‑center) + 1)   (reflected right half)
   This implements the palindromic construction from the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    idx_t size = 2;          /* L_1 */
    idx_t k = 1;
    while (size < n) {
        size = size * 2 + 1; /* L_{k+1} = 2·L_k + 1   */
        ++k;
    }

    while (size > 2) {
        idx_t center = (size + 1) / 2;  /* middle position of the block */
        if (n == center) {
            return (k & 1) ? (seq_t)1 : (seq_t)0;
        }
        if (n < center) {
            size = (size - 1) / 2;
            --k;
            continue;
        }
        /* n > center: reflect into the left part */
        n = size - n + 1;
        size = (size - 1) / 2;
        --k;
    }

    /* size == 2 – remaining indices are 1 or 2 */
    if (n == 1) return (seq_t)0;
    return (seq_t)1;
}
