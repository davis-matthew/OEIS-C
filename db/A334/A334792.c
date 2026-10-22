/*
 * A334792  Let L_0 = (0, 1, 2, ...); for k = 1, 2, ..., L_k is obtained by
 *          splitting L_{k-1} into runs of k! terms and reversing even-indexed
 *          runs; {a(n)} is the limit of L_k as k tends to infinity.
 * https://oeis.org/A334792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A334792
 * a(n) is the value at index n after repeatedly
 * reversing every odd (1‑based) block of length k! for k = 2,3,4,...
 * The function inverts the cumulative permutation by running k
 * from the largest k with k! ≤ n down to 2.
 */
seq_t a(idx_t n)
{
    /* current index in the final sequence */
    idx_t pos  = n;

    /* find the largest k with k! ≤ n, keeping the factorial in fact */
    idx_t fact = 1;   /* k! */
    idx_t k    = 1;

    while (1) {
        idx_t next   = k + 1;
        idx_t nextf  = fact * next;
        if (nextf > n) break;   /* next factorial too large */
        k    = next;
        fact = nextf;
    }

    if (k <= 1) /* no useful reversals */
        return (seq_t)pos;

    /* unwind the reversals in reverse order (k down to 2) */
    for (idx_t i = k; i >= 2; --i) {
        idx_t r   = pos / fact;          /* block number (0‑based)   */
        idx_t off = pos % fact;          /* offset inside block      */
        if (r & 1)                       /* odd block → reversed    */
            pos = (r + 1) * fact - 1 - off;
        fact /= i;                       /* go to (i-1)!            */
    }

    return (seq_t)pos;
}
