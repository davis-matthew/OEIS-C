/*
 * A312208  Coordination sequence Gal.5.69.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence A312208 (Gal.5.69.1)
   Recurrence: a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=19.
   For n ≥ 5,
      a(n) = a(n-1) + d((n-4) mod 10),
      where d = [6,6,6,5,4,4,4,4,5,6] (period 10). */
seq_t a(idx_t n)
{
    /* first 5 base values */
    static const seq_t base[5] = { (seq_t)1, (seq_t)4, (seq_t)8,
                                   (seq_t)13, (seq_t)19 };

    /* prefix sums of the 10‑period difference array */
    static const seq_t pref[11] = {
        (seq_t)0,  /* sum of 0 differences  */
        (seq_t)6,  /* 1st 6  */
        (seq_t)12, /* 2nd 6  */
        (seq_t)18, /* 3rd 6  */
        (seq_t)23, /* 4th 5  */
        (seq_t)27, /* 5th 4  */
        (seq_t)31, /* 6th 4  */
        (seq_t)35, /* 7th 4  */
        (seq_t)39, /* 8th 4  */
        (seq_t)44, /* 9th 5  */
        (seq_t)50  /* 10th 6 (full cycle) */
    };
    const idx_t PERIOD = 10;

    if (n < (idx_t)5) /* offset 0: a(0)..a(4) */
        return base[n];

    /* number of differences to add beyond a(4) */
    idx_t m = n - (idx_t)4;
    /* how many full 10‑difference cycles */
    idx_t cycles = m / PERIOD;
    /* position in the current cycle (0‑based) */
    idx_t pos = m % PERIOD;

    seq_t value = base[4];                /* a(4) = 19 */
    value += (seq_t)cycles * (seq_t)pref[PERIOD];
    value += pref[pos];
    return value;
}
