/*
 * A315229  Coordination sequence Gal.6.282.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A315229: Coordination sequence Gal.6.282.6
   Formula:
   Let k = n / 4 and r = n % 4.
   For n = 0, a(0) = 1.
   For n ≥ 1:
       base = 22 * k          // a(4k) = 22k
       a(n) = base + d(r, k)
   where
     d(0, k) = 0
     d(1, k) = { 6 - 4k            if k even
                { -8 * ((k-1)/2)     if k odd
     d(2, k) = 10 - 4*((k+1)/2) - 2*(k/2)
     d(3, k) = 14 - 6*((k+1)/2) - 2*(k/2)
*/

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t k = n / 4;
    idx_t r = n % 4;

    seq_t base = (seq_t)22 * k;
    long long d = 0;

    switch (r)
    {
        case 0:
            return base;

        case 1:
            if ((k & 1ULL) == 0ULL)
                d = 6LL - 4LL * (long long)k;
            else
                d = -8LL * ((long long)(k - 1) / 2);
            break;

        case 2:
            d = 10LL
                - 4LL * ((long long)(k + 1) / 2)
                - 2LL * (long long)(k / 2);
            break;

        case 3:
            d = 14LL
                - 6LL * ((long long)(k + 1) / 2)
                - 2LL * (long long)(k / 2);
            break;
    }

    long long res = (long long)base + d;
    return (seq_t)res;
}
