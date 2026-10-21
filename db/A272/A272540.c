/*
 * A272540  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 491", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5.
   For n ≥ 2 define r(n)=a(n)−4·a(n−1).
   Then r(1)=1, and for n ≥ 2
     r(n)=2·r(n−1)+d(n),
   where d(n) is
        10, 12, 72, –212, 100,
        –144, –88, –228, –92, …
   (from n=7 on the list –144, –88, –228, –92 repeats).
   Hence a(n)=4·a(n−1)+r(n) and can be computed iteratively. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_val = (seq_t)5;       /* a(n‑1) for the current iteration */
    seq_t r     = (seq_t)1;       /* r(n‑1)  */

    for (idx_t i = 2; i <= n; ++i)
    {
        long long d;
        switch (i)
        {
            case 2: d =  10; break;
            case 3: d =  12; break;
            case 4: d =  72; break;
            case 5: d = -212; break;
            case 6: d =  100; break;
            default:
            {
                unsigned rem = (unsigned)((i - 7) % 4);
                if (rem == 0) d = -144;
                else if (rem == 1) d = -88;
                else if (rem == 2) d = -228;
                else              d = -92;
            }
        }

        seq_t r_curr = r * 2;
        if (d >= 0)
            r_curr += (seq_t)d;
        else
            r_curr -= (seq_t)(-d);

        a_val = a_val * 4 + r_curr;
        r = r_curr;
    }

    return a_val;
}
