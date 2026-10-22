/*
 * A352406  The number of terms before reaching zero when starting at n and
 *          iterating: f(n) = n, f(n+1) = n+1; f(n+k) = (f(n+k-2) + f(n+k-1))
 *          (mod (n+k)), where k>=2.
 * https://oeis.org/A352406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of terms before the first zero occurs in
 *            the sequence defined by f(n) = n f(n+1) = n+1 f(n+k) = (f(n+k-2)
 *            + f(n+k-1)) mod (n + k) for k ≥ 2. The function returns a(idx_t
 *            n) = the count of non‑zero terms preceding the first zero
 * Range:     u8 n <= 19, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the definition literally.
   It uses only addition, comparison, and modulo on seq_t values,
   and works for any seq_t/idx_t combination that can represent
   the intermediate and final values. */

seq_t a(idx_t n)
{
    seq_t prevprev = (seq_t)n;                 /* f(n)   */
    seq_t prev    = (seq_t)(n + 1);            /* f(n+1) */
    idx_t  i      = 2;                         /* next index to compute */

    while (1)
    {
        seq_t mod  = (seq_t)(n + i);           /* modulus for f(n+i) */
        seq_t sum  = prevprev + prev;         /* temporary sum */

        /* Compute f(n+i) = sum mod mod without overflow on small types */
        seq_t cur;
        if (sum >= mod)
            cur = sum - mod;
        else
            cur = sum;

        if (cur == 0)
            return (seq_t)i;                  /* i terms before the zero */

        /* Advance to next step */
        prevprev = prev;
        prev    = cur;
        ++i;
    }
}
