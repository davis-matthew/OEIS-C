/*
 * A255810  Infinite tree (flattened) generated as follows: generation g(1) =
 *          (1); thereafter, putting h = 3^n, each (1,x(2),...,x(h)) in
 *          generation g(n) has 1st, 2nd, and 3rd offspring, namely
 *          (1,x(2),...,x(h),x(h)+1), (1,x(2),...,x(h),x(h)+2) and
 *          (1,x(2),...,h(h),x(h)+3).
 * https://oeis.org/A255810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7000, u16 n <= 7000, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255810:  a(n) is the n‑th element of the flattened list
   of increasing n‑tuples that start with 1 and have successive gaps
   of size 1, 2, or 3.  The function locates the generation that
   contains position n and evaluates the required tuple element. */
seq_t a(idx_t n)
{
    /* Locate the generation g (1‑based) that contains n. */
    idx_t total  = 0;          /* elements in previous generations */
    idx_t power  = 1;          /* 3^(g-1)  */
    idx_t g      = 1;          /* current generation                          */
    while (1) {
        idx_t remaining = n - total;          /* n > total here               */
        if (power > remaining / g) break;     /* count would exceed remaining */
        idx_t count = power * g;              /* elements in this generation  */
        if (total + count >= n) break;
        total += count;
        power *= 3;
        ++g;
    }

    /* Position of n within generation g (0‑based). */
    idx_t offset = n - total - 1;
    idx_t nodeIdx = offset / g;   /* index of the tuple within generation g */
    idx_t r       = offset % g;   /* index of the element inside that tuple */

    /* First element of every tuple is 1. */
    if (r == 0) return (seq_t)1;

    /* Recover base‑3 digits of nodeIdx (one per gap). */
    static idx_t inc[128];
    for (idx_t i = g - 1; i > 0; --i) {
        inc[i - 1] = nodeIdx % 3;
        nodeIdx   /= 3;
    }

    /* Sum the increments for the first r gaps. */
    seq_t sumInc = 0;
    for (idx_t i = 0; i < r; ++i)
        sumInc += (seq_t)inc[i];

    /* The r‑th element is 1 + r + sum of inc[0..r‑1]. */
    return (seq_t)1 + (seq_t)r + sumInc;
}
