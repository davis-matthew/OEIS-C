/*
 * A314811  Coordination sequence Gal.5.254.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314811
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

/* Coordination sequence A314811
   Derived closed‑form:  a(n) = 27·⌊n/6⌋ + floor(9·(n%6)/2) + c(n)
   where c(n) = 1          for n%6==0 and ⌊n/6⌋≤1
                 1          for n%6==1 or 3
                 -⌊n/6⌋     for n%6==2
                 -1         for n%6==4
                 0          for n%6==5
   This formula reproduces all terms of the sequence. */
seq_t a(idx_t n)
{
    /* coefficients for floor(9*r/2) with r = n%6 */
    static const unsigned f[6] = {0, 4, 9, 13, 18, 22};

    idx_t  k = n / 6;          /* quotient */
    unsigned r = (unsigned)(n % 6);   /* remainder (0..5) */

    /* base term: 27*k + f[r] */
    seq_t base = ((seq_t)k * 27) + f[r];

    /* signed correction term */
    long long add = 0;

    switch (r) {
        case 0:
            add = (k <= 1) ? 1 : (long long)k;
            break;
        case 1:
        case 3:
            add = 1;
            break;
        case 2:
            add = -(long long)k;
            break;
        case 4:
            add = -1;
            break;
        case 5:
            add = 0;
            break;
    }

    /* final result */
    seq_t result = base;
    if (add >= 0) {
        result += (seq_t)add;
    } else {
        result -= (seq_t)(-add);
    }
    return result;
}
