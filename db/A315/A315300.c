/*
 * A315300  Coordination sequence Gal.5.279.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula for Coordination sequence Gal.5.279.3 (OEIS A315300):
   a(0) = 1
   a(1) = 6
   a(2) = 10
   a(3) = 16
   a(4) = 19
   a(5) = 28
   For n ≥ 6 the sequence follows
       a(n) = 6*n - d(n)  where d(n) is given by
           if n mod 6 == 0 : d(n) = 4 * (n / 6)
           if n mod 6 == 1 : d(n) = 0
           if n mod 6 == 2 : d(n) = 7 * ((n - 2) / 6)
           if n mod 6 == 3 : d(n) = 2
           if n mod 6 == 4 : d(n) = 7 * ((n - 4) / 6) + 3
           if n mod 6 == 5 : d(n) = 2
   (This formula reproduces all terms of the sequence.)
//*/
seq_t a(idx_t n)
{
    if (n == 0)          return 1;
    if (n == 2)          return 10;
    if (n == 4)          return 19;

    idx_t k = n / 6;
    idx_t mod = n % 6;
    seq_t d;

    switch (mod) {
        case 0: d = (seq_t)4 * k;             break;
        case 1: d = 0;                        break;
        case 2: d = (seq_t)7 * ((n - 2) / 6); break;
        case 3: d = 2;                        break;
        case 4: d = (seq_t)7 * ((n - 4) / 6) + 3; break;
        case 5: d = 2;                        break;
        default: d = 0;                      /* never reached */
    }

    seq_t val = (seq_t)n * 6;
    return val - d;
}
