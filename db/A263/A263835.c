/*
 * A263835  Sprague-Grundy sequence G(i-Mark({4,8}, {2})).
 * https://oeis.org/A263835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { for n>=17 0 if n%12 in {1,3,4,10} 1 if n%12 in
 *            {5,6,7,8} 2 if n%12 in {2,9,11} 2 if n%12==0 and v2(n/12) even 3
 *            if n%12==0 and v2(n/12) odd a(0..16) given by initial table
 *            above } This matches OEIS A263835
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values 0..16 */
    static const seq_t init[17] = {
        0, 0, 1, 0, 2, 1, 2, 1, 1, 2, 0, 2, 0, 0, 3, 0, 2
    };
    if (n < 17)
        return init[n];
    idx_t r = n % 12;
    switch (r) {
    case 1: case 3: case 4: case 10:
        return 0;
    case 5: case 6: case 7: case 8:
        return 1;
    case 2: case 9: case 11:
        return 2;
    case 0: {
        idx_t k = n / 12;
        unsigned e = 0;
        while ((k & 1) == 0) {
            e++;
            k >>= 1;
        }
        return (e & 1) ? (seq_t)3 : (seq_t)2;
    }
    default:
        /* unreachable */
        return 0;
    }
}
