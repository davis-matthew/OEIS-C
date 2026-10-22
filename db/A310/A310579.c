/*
 * A310579  Coordination sequence Gal.4.15.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1: a(n) = a(n‑1) + d(n), where d(1)=3, d(2)=2,
 *            d(3)=4, d(4)=2, d(5)=4, d(6)=2. For n ≥ 7: let k = (n‑7)/6 and r
 *            = n % 6: if r==1: d(n)= 4*(k+3) if r==2: d(n)= -4*(k+1) if r==3:
 *            d(n)= 4 if r==4: d(n)= 2 if r==5: d(n)= 4 if r==0: d(n)= 2 This
 *            recurrence reproduces OEIS A310579
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Use signed long long for intermediate sums and differences,
       which are sufficient for n up to at least 10^7. */
    long long acc = 1;   /* corresponds to a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        long long d = 0;
        if (i == 1)         d = 3;
        else if (i == 2)    d = 2;
        else if (i == 3)    d = 4;
        else if (i == 4)    d = 2;
        else if (i == 5)    d = 4;
        else if (i == 6)    d = 2;
        else { /* i >= 7 */
            idx_t k = (i - 7) / 6;
            idx_t r = i % 6;
            if (r == 1)                     d = 4LL * (long long)(k + 3);
            else if (r == 2)                d = -4LL * (long long)(k + 1);
            else if (r == 3)                d = 4LL;
            else if (r == 4)                d = 2LL;
            else if (r == 5)                d = 4LL;
            else /* r == 0 */               d = 2LL;
        }
        acc += d;
    }
    return (seq_t)acc;
}
