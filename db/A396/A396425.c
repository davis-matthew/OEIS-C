/*
 * A396425  a(n) = r(n,n), where r(1,k)=f(k)=round(k*c), r(n,k)=f(r(n-1,k)),
 *          c=5/3.
 * https://oeis.org/A396425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) where r(1,k) = round(k * 5/3), r(n,k) =
 *            round(r(n‑1,k) * 5/3), and round(x*5/3) = floor((5*x + 1)/3).
 *            Initial value: a(1) = 2
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n ≥ 1 compute r(n,n) by applying the rounding R(x)=floor((5*x+1)/3)
       n times to the starting value x = n.  The 128‑bit intermediate prevents
       overflow for builtin integer types while still compiling for the
       arbitrary‑precision class (the cast is used only when the underlying
       type is a builtin). */
    seq_t x = (seq_t)n;            /* start with k = n */
    for (idx_t i = 0; i < n; ++i) {
        unsigned __int128 t = (unsigned __int128)x * 5 + 1;
        x = (seq_t)(t / 3);        /* integer division truncates toward 0 */
    }
    return x;
}
