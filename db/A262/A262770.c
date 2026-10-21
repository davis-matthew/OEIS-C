/*
 * A262770  A Beatty sequence: a(n)=floor(n*p) where p=2*cos(Pi/7)=A160389.
 * https://oeis.org/A262770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n * p ) with p = 2 * cos(π/7) (the shorter
 *            diagonal in a regular heptagon)
 * Range:     u8 n <= 100, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute the Beatty sequence term a(n) with high precision. */
seq_t a(idx_t n)
{
    /* Handle the base case.  The OEIS offset is 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute p = 2*cos(π/7) as a long double. */
    /* acosl(-1.0L) gives π accurately in long double. */
    long double pi = acosl(-1.0L);
    long double p  = 2.0L * cosl(pi / 7.0L);

    /* Find the floor of p (integer part). */
    idx_t a0 = (idx_t)p;          /* a0 = ⌊p⌋ = 1 for p ≈ 1.8… */

    /* Continued‑fraction quotients for p.
       We'll generate them until the denominator of the convergent
       exceeds n.  The error of such a convergent guarantees that
       floor(n*p) = floor(n*h/k). */
    unsigned long long a = a0;          /* current CF coefficient */
    unsigned __int128 h_prev2 = 0, h_prev1 = 1;   /* numerators */
    unsigned __int128 k_prev2 = 1, k_prev1 = 0;   /* denominators */

    long double frac = p - (long double)a0;       /* fractional part of p */
    /* In rare cases the fractional part may become zero due to rounding;
       guard against that. */
    if (frac <= 0.0L) frac = 1e-30L;

    /* Current convergent h/k */
    unsigned __int128 h, k;

    for (;;)
    {
        /* Compute the next convergent.  (h,k) = a*h_prev1 + h_prev2,  a*k_prev1 + k_prev2 */
        h = a * h_prev1 + h_prev2;
        k = a * k_prev1 + k_prev2;

        /* When k > n the error of this convergent is < 1/(k·k_next),
           thus n*(p - h/k) < 1, guaranteeing floor(n*p) = floor(n*h/k).  */
        if ( (unsigned __int128)k > (unsigned __int128)n )
        {
            seq_t sn    = (seq_t)n;
            seq_t h_seq = (seq_t)h;
            seq_t k_seq = (seq_t)k;
            /* Integer division gives the required floor. */
            return (seq_t)( (sn * h_seq) / k_seq );
        }

        /* Prepare for next step: update previous convergents. */
        h_prev2 = h_prev1;  h_prev1 = h;
        k_prev2 = k_prev1;  k_prev1 = k;

        /* Compute next CF coefficient a = ⌊1/frac⌋. */
        long double inv = 1.0L / frac;
        a = (unsigned long long)inv;
        if (a == 0)          /* this should not happen for irrational p */
            a = 1;
        frac = inv - (long double)a;
        /* Protect against the fractional part becoming 0 due to rounding. */
        if (frac <= 0.0L)
            frac = 1e-30L;
    }
}
