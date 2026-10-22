/*
 * A385177  a(n) = Sum_{k=1..n} ceiling(k/phi), where phi is the golden ratio
 *          (A001622).
 * https://oeis.org/A385177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} ceil(k/phi) (phi = (1+sqrt5)/2)
 * Range:     u8 n <= 27, u16 n <= 459, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* the sequence is defined for n≥1 */

    /* Find the first Fibonacci pair F_{m+1}/F_m   with F_m > n
       The convergent F_{m+1}/F_m approximates phi very well.
       For any k < F_m, floor(k*phi) == floor(k*F_{m+1}/F_m).   */
    idx_t prev = 1;          /* F_{m-1} */
    idx_t curr = 1;          /* F_m   */
    while (curr <= n) {
        idx_t next = prev + curr;   /* F_{m+1} */
        prev = curr;
        curr = next;
    }
    /* Now curr (=F_m) > n.  The next Fibonacci number is prev + curr (=F_{m+1}) */
    idx_t q = curr;                    /* denominator of the convergent (F_m) */
    idx_t p = prev + curr;             /* numerator of the convergent (F_{m+1}) */

    seq_t qs = (seq_t)q;
    seq_t ps = (seq_t)p;

    /* Compute S(n) = sum_{k=1..n} floor(k * phi)  by using the convergent */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += ((seq_t)k * ps) / qs;

    /* a(n) = S(n) - n*(n-1)/2   (derived below)
       Proof:
         ceil(k/phi) = floor(k/phi) + 1
                     = (floor(k*phi) - k) + 1          (since floor(k*phi)=k+floor(k/phi))
                     = floor(k*phi) - k + 1
         a(n) = Σ [floor(k*phi) - k + 1]
              = Σ floor(k*phi)  - Σ k  + n
              = S(n) - n*(n+1)/2 + n
              = S(n) - n*(n-1)/2
    */
    seq_t nseq = (seq_t)n;
    seq_t sub = nseq * (nseq - 1) / 2;
    return sum - sub;
}
