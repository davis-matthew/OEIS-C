/*
 * A289794  Number of 6-cycles in the n-tetrahedral graph.
 * https://oeis.org/A289794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*binomial(n,5)*(454 - 409*n + 66*n^2 + n^3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the first 9 terms by the formula and
   then iterates using the 9‑term linear recurrence.  The recurrence
   is rewritten as a sum of positives minus a sum of negatives so
   that unsigned seq_t types never perform a subtraction that could
   under‑flow.  The routine works for all n ≥ 1 and stays well under
   2 s for n up to about 10 000. */

#include <stdint.h>
#include <stdlib.h>

/*   a(n) = 5*binomial(n,5)*(454 - 409*n + 66*n^2 + n^3)
    Rewritten recurrence (valid for n ≥ 10):
       a(n) =  9a(n‑1) -36a(n‑2) +84a(n‑3) -126a(n‑4) +126a(n‑5)
             -84a(n‑6) +36a(n‑7) -9a(n‑8) +a(n‑9)
   This is expressed as
       a(n) = (9a(n-1)+84a(n-3)+126a(n-5)+36a(n-7)+a(n-9))
              - (36a(n-2)+126a(n-4)+84a(n-6)+9a(n-8))
*/

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;

    /* Allocate array of size n+1.  Index 0 is unused.  */
    seq_t *a = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a)
        return (seq_t)0;      /* allocation failure → zero */

    /* First 9 terms computed directly from the formula (safe for all
       small n because intermediate values fit in 64 bits). */
    for (idx_t i = 1; i <= n && i <= 9; ++i)
    {
        seq_t nseq = (seq_t)i;

        /* binomial(i,5) computed multiplicatively: i*(i-1)*.../(5!) */
        seq_t bin = (seq_t)1;
        for (int k = 0; k < 5; ++k)
            bin = bin * (nseq - (seq_t)k) / (seq_t)(k + 1);

        seq_t n2 = nseq * nseq;
        seq_t n3 = n2 * nseq;

        /* polynomial: 454 - 409*n + 66*n^2 + n^3
           compute as sum of positives minus one negative part
           to avoid underflow with unsigned seq_t */
        seq_t poly = (seq_t)454 + (seq_t)66 * n2 + n3;
        poly -= (seq_t)409 * nseq;

        /* a(i) = 5 * bin * poly */
        a[i] = (seq_t)5 * (bin * poly);
    }

    /* For i ≥ 10 use the recurrence expressed with only additions
       and a single subtraction of two positive sums. */
    for (idx_t i = 10; i <= n; ++i)
    {
        seq_t pos = (seq_t)9 * a[i - 1];
        pos += (seq_t)84 * a[i - 3];
        pos += (seq_t)126 * a[i - 5];
        pos += (seq_t)36 * a[i - 7];
        pos += a[i - 9];

        seq_t neg = (seq_t)36 * a[i - 2];
        neg += (seq_t)126 * a[i - 4];
        neg += (seq_t)84 * a[i - 6];
        neg += (seq_t)9 * a[i - 8];

        a[i] = pos - neg;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
