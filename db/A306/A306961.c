/*
 * A306961  Trajectory of 3 under repeated application of x -> A306958(x).
 * https://oeis.org/A306961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Sequence A306961 – trajectory of 3 under repeated application of A306958.
   The first few terms are:
     a(1)=3, a(2)=720, a(3)=604891
   Starting at a(4) the sequence enters a cycle of length 26:
     (5599451, 7353370, 1242001, 5242, 35460, 187201,
      2419311, 3634680, 2123281, 1815410, 1849711, 6053070,
      786963, 6351120, 182271, 2419400, 3638982, 7410330,
      611292, 3780200, 2420013, 5952, 3689370, 6200641,
      307542, 640891)
   Hence for n ≥ 4:  a(n) = cycle[(n-4) % 26].
*/

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)720;
    if (n == 3) return (seq_t)604891;

    static const seq_t cycle[26] = {
        (seq_t)5599451, (seq_t)7353370, (seq_t)1242001, (seq_t)5242,
        (seq_t)35460,   (seq_t)187201,   (seq_t)2419311, (seq_t)3634680,
        (seq_t)2123281, (seq_t)1815410,   (seq_t)1849711, (seq_t)6053070,
        (seq_t)786963,  (seq_t)6351120,   (seq_t)182271,  (seq_t)2419400,
        (seq_t)3638982, (seq_t)7410330,   (seq_t)611292,  (seq_t)3780200,
        (seq_t)2420013, (seq_t)5952,     (seq_t)3689370, (seq_t)6200641,
        (seq_t)307542,  (seq_t)640891
    };

    idx_t idx = (n - 4) % 26;   // n >= 4 guaranteed
    return cycle[idx];
}
