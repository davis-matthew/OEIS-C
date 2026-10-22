/*
 * A315106  Coordination sequence Gal.5.256.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=9, a(3)=16, a(4)=21, a(5)=25; * for n>=6,
 *            a(n)=a(n-1)+(pattern[(n-6) mod 5]) where pattern = {7,3,7,6,3} *
 *            (i.e. a(n)=25 + ((n-5)/5)*26 + sum of first ((n-5)%5) elements
 *            of the pattern)
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t first[6] = { (seq_t)1, (seq_t)5, (seq_t)9,
                                   (seq_t)16, (seq_t)21, (seq_t)25 };
    if (n < 6) return first[n];

    /* pattern of successive differences starting at n=6 */
    static const int pattern[5] = {7,3,7,6,3};

    idx_t m = n - 5;               /* number of differences to add after a(5) */
    idx_t cycles = m / 5;           /* full periods of the pattern */
    int rem = (int)(m % 5);         /* remaining elements of the pattern */

    seq_t result = (seq_t)25;        /* start from a(5) */
    result += (seq_t)cycles * (seq_t)26;  /* sum of full periods */

    int sum = 0;                    /* sum of the remaining pattern entries */
    for (int i = 0; i < rem; ++i) sum += pattern[i];
    result += (seq_t)sum;           /* add the remainder */

    return result;
}
